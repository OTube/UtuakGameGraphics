#include "main.h"

#define CONVERT(x,y,sizex) (y)*sizex+(x)
#define COMMAND(cmd) commands[e][cmd]

unsigned int encode(unsigned char *userdata, char *to, unsigned int rsize){
    FILE *fl2 = fopen(to, "wb");
    unsigned int qr = rsize*rsize;
    unsigned char prev[4];
    unsigned char s = 0;
    unsigned int fsize = 0;
    prev[0] = userdata[0];
    prev[1] = userdata[1];
    prev[2] = userdata[2];
    prev[3] = userdata[3];
    unsigned int f = 1;
    for(int h = 0; h < qr; ++h){
        unsigned int is_repeat =
            prev[0] == userdata[h*4+0] &&
            prev[1] == userdata[h*4+1] &&
            prev[2] == userdata[h*4+2] &&
            prev[3] == userdata[h*4+3];
        if(!is_repeat || s == 255){
            if(f){
                --s;
                f = 0;
            }
            fwrite(&s, sizeof(unsigned char), 1, fl2);
            fwrite(prev, 4, 1, fl2);
            fsize += 5;
            prev[0] = userdata[h*4+0];
            prev[1] = userdata[h*4+1];
            prev[2] = userdata[h*4+2];
            prev[3] = userdata[h*4+3];
            s = 0;
        }else{
            ++s;
        }
    }
    fclose(fl2);
    return fsize;
}

int main(int argc, char **argv){
	if(argc < 2) return 1;
	int IMAGE_SIZE_INT;
	float IMAGE_SIZE;
	sscanf(argv[1], "%d", &IMAGE_SIZE_INT);
	if(!IMAGE_SIZE_INT)return 1;
	IMAGE_SIZE = (float)IMAGE_SIZE_INT;
	char names[32][16];
	unsigned int names_c = 0, len;
	FILE *conf = fopen("conf.txt", "r");
	if(conf == NULL) return 1;
	char relative_dir[64];
	if(fgets(relative_dir, 64, conf)==NULL){
		return 1;
	}
	for(int g = 0; g < 64; ++g){
		if(relative_dir[g] == ';'){
			relative_dir[g] = '\0';
			break;
		}
	}
	char cmd, cmd2;
	int commands[32][4];
	for(int i = 0; i < 32; ++i){
		commands[i][0] = 0;
		commands[i][1] = 0;
		commands[i][2] = 0;
		commands[i][3] = 0;
	}
	while(1){
		memset(names[names_c], '\0', 16);
		//memset(total_map[names_c], '\0', 32);
		char c_md;
		unsigned int len = 0;
		while(1){
			c_md = (char)fgetc(conf);
			if(c_md == ' ' || c_md == '\n') continue;
			if((c_md == EOF) || (c_md < 0) || (c_md == ';')) break;
			names[names_c][len++] = c_md;
		}
		if(len < 1) break;
		//if(fgets(names[names_c], 16, conf)==NULL)break;
		//names[names_c][strcspn(names[names_c], "\n")] = '\0';
		char buffer[8] = {};
		for (int i = 0; i < 2; ++i){
			int c = 0;
			while(c < 8 && ((buffer[c] = (char)fgetc(conf)) != '|')) ++c;
			int res;
			sscanf(buffer, "%d", &res);
			//printf("biasx: %d\n", res);
			commands[names_c][i] = res;
		}
		for (int i = 2; i < 4; ++i){
			commands[names_c][i] = fgetc(conf) == '^';
		}
		++names_c;
		if((c_md == EOF) || (c_md < 0)) break;
	}
	fclose(conf);
	BITMAP maps[names_c];
	char formatted[1024];
	for(int i = 0; i < names_c; ++i){
		snprintf(formatted, 1024, "%s/%s.bmp", relative_dir, names[i]);
		//puts(formatted);
		FILE *mapf = fopen(formatted, "rb");
		if(mapf == NULL){
			printf("not found:|%s|%s|\n", formatted, names[i]);
			return 1;
		}
		BITMAP *mp = maps + i;
		fread(&mp->fh, sizeof(FILEHEADER), 1, mapf);
		fread(&mp->ih, sizeof(INFOHEADER), 1, mapf);
		//mp->bias = biases[i].x != -1 ? biases[i] : (pointer){0,0};
		mp->data = (color*)malloc(mp->ih.width*mp->ih.height*sizeof(color));
		fseek(mapf, mp->fh.imageDataOffset, SEEK_SET);
		fread(mp->data,mp->ih.width*mp->ih.height*sizeof(color),1,mapf);
		fclose(mapf);
		//puts(names[i]);
	}
	printf("count: %d\n", names_c);
	color *image = (color*)malloc(IMAGE_SIZE_INT*IMAGE_SIZE_INT*sizeof(color));
	memset(image, '\0', IMAGE_SIZE_INT*IMAGE_SIZE_INT*sizeof(color));
	markdown mk[names_c];
	unsigned int best_y = 0;
	for(int e = 0; e < names_c; ++e){
		BITMAP *current = maps + e;
		pointer point = {
			COMMAND(2) ?
				mk[COMMAND(0)].position.x+
				mk[COMMAND(0)].scale.x:
				COMMAND(0),
			COMMAND(3) ?
				mk[COMMAND(1)].position.y+
				mk[COMMAND(1)].scale.y:
				COMMAND(1)
		};
		printf("sizes: %d|%d\n", current->ih.width, current->ih.height);
		printf("point: %d|%d\n", point.x, point.y);
		//point = (pointer){e%4*32,e/4*32-1};
		mk[e].position = (pointer){point.x, point.y};
		mk[e].scale = (pointer){current->ih.width, current->ih.height};
		for(int i = 0; i < current->ih.width; ++i){
			int mt1 = i+point.x;
			if(mt1 > IMAGE_SIZE_INT || mt1 < 0) continue;
			for(int j = 0; j < current->ih.height; ++j){
				int mt2 = j+point.y;
				if(mt2 > IMAGE_SIZE_INT || mt2 < 0) continue;
				color nxt = current->data[CONVERT(i,current->ih.height-j-1,current->ih.width)];
				if(nxt.a == 0){
					for (int i1 = i-1; i1 < i+2; ++i1){
						if(i1 < 0 || i1 >= current->ih.width) continue;
						for (int j1 = j-1; j1 < j+2; ++j1){
							if(i1==i&&j1==j)continue;
							if(j1 < 0 || j1 >= current->ih.height) continue;
							color nnxt = current->data[CONVERT(i1,current->ih.height-j1-1,current->ih.width)];
							if(nnxt.a){
								nxt.r = nnxt.r;
								nxt.g = nnxt.g;
								nxt.b = nnxt.b;
								break;
							}
						}
					}
				}
				image[CONVERT(i+point.x,(j+point.y),IMAGE_SIZE_INT)] = (color){
					nxt.b,
					nxt.g,
					nxt.r,
					nxt.a
				};
			}
		}
	}
	FILE *mk_file = fopen("markdown.txt", "w");
	vec2 verts[4];
	for(int e = 0; e < names_c; ++e){
		markdown cm = mk[e];
		verts[0] = (vec2){cm.position.x, cm.position.y + cm.scale.y};
		verts[1] = (vec2){cm.position.x + cm.scale.x, cm.position.y + cm.scale.y};
		verts[2] = (vec2){cm.position.x + cm.scale.x, cm.position.y};
		verts[3] = (vec2){cm.position.x, cm.position.y};
		for(int h = 0; h < 4; ++h){
			verts[h].x /= IMAGE_SIZE;
			verts[h].y /= IMAGE_SIZE;
			//verts[h].x = 1.0f - verts[h].x;
			//verts[h].y = 1.0f - verts[h].y;
		}
		fprintf(mk_file, "%ff,%ff,%ff,%ff,%ff,%ff,%ff,%ff,\n", verts[0].x,verts[0].y, verts[1].x,verts[1].y, verts[2].x,verts[2].y, verts[3].x,verts[3].y);
	}
	fclose(mk_file);
	puts("markdown generated to markdown.txt");
	printf("count: %d\n", names_c);
	encode((unsigned char *)image, "/home/oleg/Desktop/gametex.ugg", IMAGE_SIZE_INT);
	//FILE *write_to = fopen("/home/oleg/Desktop/gametex.utx", "wb");
	//fwrite(image, IMAGE_SIZE_INT*IMAGE_SIZE_INT*sizeof(color), 1, write_to);
	//fclose(write_to);
	free(image);
	return 0;
}
