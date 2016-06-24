#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <GL/gl.h>
#include <GL/glut.h>
#include <vector>
#include <iostream>

#include "teste.h"

using namespace std;

vector<OBJ*> _objetos(0);

vector<MAT*> _materiais(0);

vector<TEX*> _texturas(0);

int _procuraMaterial(char *nome) {
    unsigned int i;
    for (i = 0; i < _materiais.size(); ++i)
        if (!strcmp(nome, _materiais[i]->nome))
            return i;
    return -1;
}

void _leMateriais(char *nomeArquivo) {
    char aux[256];
    FILE *fp;
    MAT *ptr;
    fp = fopen(nomeArquivo, "r");
    if (fp == NULL) {
        printf("Problema:%s\n", nomeArquivo);
    }

    while (!feof(fp)) {
        fgets(aux, 255, fp);

        if (strlen(aux) > 1)
            aux[strlen(aux) - 1] = 0;
        if (aux[0] == '#') continue;

        if (!strncmp(aux, "newmtl", 6)) {
            if (_procuraMaterial(&aux[7]) != -1) {
                ptr = NULL;
                continue;
            }
            if ((ptr = (MAT *) malloc(sizeof (MAT))) == NULL) {
                printf("Sem mem�ria para novo material!");
                exit(1);
            }
            _materiais.push_back(ptr);
            strcpy(ptr->nome, &aux[7]);
            ptr->ke[0] = ptr->ke[1] = ptr->ke[2] = 0.0;
        }
        if (!strncmp(aux, "Ka ", 3)) // Ambiente
        {
            if (ptr == NULL) continue;
            sscanf(aux, "Ka %f %f %f", &ptr->ka[0], &ptr->ka[1], &ptr->ka[2]);
        }
        if (!strncmp(aux, "Kd ", 3)) // Difuso
        {
            if (ptr == NULL) continue;
            sscanf(aux, "Kd %f %f %f", &ptr->kd[0], &ptr->kd[1], &ptr->kd[2]);
        }
        if (!strncmp(aux, "Ks ", 3)) // Especular
        {
            if (ptr == NULL) continue;
            sscanf(aux, "Ks %f %f %f", &ptr->ks[0], &ptr->ks[1], &ptr->ks[2]);
        }
        if (!strncmp(aux, "Ns ", 3)) // Fator de especularidade
        {
            if (ptr == NULL) continue;
            sscanf(aux, "Ns %f", &ptr->spec);
            // Converte da faixa lida (0...1000) para o intervalo
            // v�lido em OpenGL (0...128)
            ptr->spec = ptr->spec / 1000.0 * 128;
        }
        if (!strncmp(aux, "d ", 2)) // Alpha
        {
            if (ptr == NULL) continue;
            float alpha;
            sscanf(aux, "d %f", &alpha);
            ptr->ka[3] = alpha;
            ptr->kd[3] = alpha;
            ptr->ks[3] = alpha;
        }
    }
    fclose(fp);
}

int leNum(char **face, char *sep) {
    char temp[10];
    int pos = 0;
    while (**face) {
        if (**face == '/' || **face == ' ') // achamos o separador
        {
            *sep = **face;
            (*face)++;
            break;
        }
        temp[pos++] = **face;
        (*face)++;
    }
    temp[pos] = 0;
    if (!pos) // string vazia ?
        return -1;
    return atoi(temp);
}

OBJ *loadObject(char *nomeArquivo) {
    int i;
    int vcont, ncont, fcont, tcont;
    int material, texid;
    char aux[256];
    TEX *ptr = NULL;
    FILE *fp;
    OBJ *obj;

    fp = fopen(nomeArquivo, "r");

    if (fp == NULL)
        return NULL;

    if ((obj = (OBJ *) malloc(sizeof (OBJ))) == NULL)
        return NULL;

    obj->numVertices = 0;
    obj->numFaces = 0;
    obj->numNormais = 0;
    obj->numTexcoords = 0;
    obj->normais_por_vertice = false;
    obj->tem_materiais = false;
    obj->textura = -1;
    obj->dlist = -1;

    obj->vertices = NULL;
    obj->faces = NULL;
    obj->normais = NULL;
    obj->texcoords = NULL;

    while (!feof(fp)) {
        fgets(aux, 255, fp);
        if (!strncmp(aux, "v ", 2))
            obj->numVertices++;
        if (!strncmp(aux, "f ", 2))
            obj->numFaces++;
        if (!strncmp(aux, "vn ", 3))
            obj->numNormais++;
        if (!strncmp(aux, "vt ", 3))
            obj->numTexcoords++;
    }
    rewind(fp);

#ifdef DEBUG
    printf("Vertices: %d\n", obj->numVertices);
    printf("Faces:    %d\n", obj->numFaces);
    printf("Normais:  %d\n", obj->numNormais);
    printf("Texcoords:%d\n", obj->numTexcoords);
#endif

    if ((obj->vertices = (VERT *) malloc((sizeof (VERT)) * obj->numVertices)) == NULL)
        return NULL;

    if ((obj->faces = (FACE *) malloc((sizeof (FACE)) * obj->numFaces)) == NULL)
        return NULL;

    if (obj->numNormais)
        if ((obj->normais = (VERT *) malloc((sizeof (VERT)) * obj->numNormais)) == NULL)
            return NULL;

    if (obj->numTexcoords)
        if ((obj->texcoords = (TEXCOORD *) malloc((sizeof (TEXCOORD)) * obj->numTexcoords)) == NULL)
            return NULL;

    vcont = 0;
    ncont = 0;
    tcont = 0;
    fcont = 0;
    material = -1;
    texid = -1;

    float minx, miny, minz;
    float maxx, maxy, maxz;


    while (!feof(fp)) {
        fgets(aux, 255, fp);
        aux[strlen(aux) - 1] = 0;
        if (aux[0] == '#') continue;
        if (!strncmp(aux, "mtllib", 6)) {
            char *matFile = new char[strlen(aux) - 6];
            strcpy(matFile, &aux[7]);
            _leMateriais(matFile);
            obj->tem_materiais = true;
        }

        if (!strncmp(aux, "usemtl", 6)) {
            material = _procuraMaterial(&aux[7]);
            texid = -1;
        }

        if (!strncmp(aux, "usemat", 6)) {
            if (!strcmp(&aux[7], "(null)")) {
                texid = -1;
                continue;
            }
            texid = ptr->texid;
        }
        if (!strncmp(aux, "v ", 2)) {
            sscanf(aux, "v %f %f %f", &obj->vertices[vcont].x,
                    &obj->vertices[vcont].y,
                    &obj->vertices[vcont].z);
            if (!vcont) {
                minx = maxx = obj->vertices[vcont].x;
                miny = maxy = obj->vertices[vcont].y;
                minz = maxz = obj->vertices[vcont].z;
            } else {
                if (obj->vertices[vcont].x < minx) minx = obj->vertices[vcont].x;
                if (obj->vertices[vcont].y < miny) miny = obj->vertices[vcont].y;
                if (obj->vertices[vcont].z < minz) minz = obj->vertices[vcont].z;
                if (obj->vertices[vcont].x > maxx) maxx = obj->vertices[vcont].x;
                if (obj->vertices[vcont].y > maxy) maxy = obj->vertices[vcont].y;
                if (obj->vertices[vcont].z > maxz) maxz = obj->vertices[vcont].z;
            }
            vcont++;
        }
        if (!strncmp(aux, "vn ", 3)) {
            sscanf(aux, "vn %f %f %f", &obj->normais[ncont].x,
                    &obj->normais[ncont].y,
                    &obj->normais[ncont].z);
            ncont++;
            obj->normais_por_vertice = true;
        }

        if (!strncmp(aux, "vt ", 3)) {
            sscanf(aux, "vt %f %f %f", &obj->texcoords[tcont].s,
                    &obj->texcoords[tcont].t,
                    &obj->texcoords[tcont].r);
            tcont++;
        }

        if (!strncmp(aux, "f ", 2)) {

            char *ptr = aux + 2;
            obj->faces[fcont].mat = material;
            obj->faces[fcont].texid = texid;
            int vi[10], ti[10], ni[10];
            char sep;
            obj->faces[fcont].nv = 0;
            int nv = 0;
            bool tem_t = false;
            bool tem_n = false;

            while (*ptr) {
                vi[nv] = leNum(&ptr, &sep);
                if (sep == ' ') {
                    nv++;
                    continue;
                }
                int aux = leNum(&ptr, &sep);
                if (aux != -1) {
                    ti[nv] = aux;
                    tem_t = true;
                }
                if (sep == ' ') {
                    nv++;
                    continue;
                }
                aux = leNum(&ptr, &sep);
                if (aux != -1) {
                    ni[nv] = aux;
                    tem_n = true;
                }
                nv++;
            }

            obj->faces[fcont].nv = nv;
            obj->faces[fcont].vert = (int *) malloc(sizeof (int)*nv);
            if (tem_n) obj->faces[fcont].norm = (int *) malloc(sizeof (int)*nv);
            else obj->faces[fcont].norm = NULL;
            if (tem_t) obj->faces[fcont].tex = (int *) malloc(sizeof (int)*nv);
            else obj->faces[fcont].tex = NULL;
            for (i = 0; i < nv; ++i) {
                obj->faces[fcont].vert[i] = vi[i] - 1;
                if (tem_n) obj->faces[fcont].norm[i] = ni[i] - 1;
                if (tem_t) obj->faces[fcont].tex[i] = ti[i] - 1;
            }

            fcont++;
        }
    }
#ifdef DEBUG
    printf("Limites: %f %f %f - %f %f %f\n", minx, miny, minz, maxx, maxy, maxz);
#endif
    fclose(fp);
    _objetos.push_back(obj);
    return obj;
}