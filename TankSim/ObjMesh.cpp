#ifdef __APPLE__
    #include <OpenGL/glu.h>
    #include <OpenGL/glu.h>
#else
    #include <windows.h>
    #include <gl/gl.h>
    #include <gl/glu.h>
#endif

#include <fstream>
#include <sstream>
#include <istream>
#include <cmath>
#include <stdio.h>
#include <vector>
#include "VECTOR3D.h"
#include "ObjMesh.h"

using namespace std;

ObjMesh :: ObjMesh (vector<VECTOR3D> &vertices,vector<VECTOR3D> &normals,vector<GLuint> &indices, vector<GLuint> &normal_indices){
    this->num_of_vertices = (int) vertices.size();
    this->num_of_normals = (int) normals.size();
    this->num_of_indices = (int) indices.size();
    
    this->vertices = &vertices[0];
    this->normals = &normals[0];
    this->indices = &indices[0];
    this->normal_indices = &normal_indices[0];
    
    this->mat_ambient[0] = 0.0;
    this->mat_ambient[1] = 0.05;
    this->mat_ambient[2] = 0.0;
    this->mat_ambient[3] = 1.0;
    this->mat_specular[0] = 0.0;
    this->mat_specular[1] = 0.0;
    this->mat_specular[2] = 0.004;
    this->mat_specular[3] = 1.0;
    this->mat_diffuse[0] = 0.5;
    this->mat_diffuse[1] = 0.5;
    this->mat_diffuse[2] = 0.5;
    this->mat_diffuse[3] = 1.0;
    this->mat_shininess[0] = 0;
    
    this->translation.x = this->translation.y = this->translation.z = 0.0f;
    this->scaleFactor.x = this->scaleFactor.y = this->scaleFactor.z = 1.0f;
    this->angles.x = this->angles.y = this->angles.z = 0.0f;
    
    this->immediate_render = true;
};

void ObjMesh :: draw() {
    //Material Properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, this->mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, this->mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, this->mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, this->mat_shininess);
    
    glPushMatrix();
    
    //Transformations
    glTranslatef(this->translation.x, this->translation.y, this->translation.z);
    glRotatef(this->angles.y, 0, 1, 0);
    glRotatef(this->angles.x, 1, 0, 0);
    glRotatef(this->angles.z, 0, 0, 1);
    glScalef(this->scaleFactor.x, this->scaleFactor.y, this->scaleFactor.z);
    
    //Rendering
    if(immediate_render) {
        
        glBindTexture(GL_TEXTURE_2D, this->textureID);
        glBegin(GL_QUADS);
        for(int i = 0; i < num_of_indices; i += 4){
            
            glNormal3f(this->normals[this->normal_indices[i]].x,
                       this->normals[this->normal_indices[i]].y,
                       this->normals[this->normal_indices[i]].z);
            
            glTexCoord2f( 1.0, 0.0);
            glVertex3f(this->vertices[this->indices[i]].x,
                       this->vertices[this->indices[i]].y,
                       this->vertices[this->indices[i]].z);
            
            glTexCoord2f( 1.0, 1.0);
            glVertex3f(this->vertices[this->indices[i+1]].x,
                       this->vertices[this->indices[i+1]].y,
                       this->vertices[this->indices[i+1]].z);
            
            glTexCoord2f( 1.0, 0.0);
            glVertex3f(this->vertices[this->indices[i+2]].x,
                       this->vertices[this->indices[i+2]].y,
                       this->vertices[this->indices[i+2]].z);
            
            glTexCoord2f( 0.0, 0.0);
            glVertex3f(this->vertices[this->indices[i+3]].x,
                       this->vertices[this->indices[i+3]].y,
                       this->vertices[this->indices[i+3]].z);
        }
        
        glEnd();
    } else {
        glEnableClientState(GL_VERTEX_ARRAY);
        
        glVertexPointer(3, GL_FLOAT, sizeof(VECTOR3D), this->vertices);
        glNormalPointer(GL_FLOAT,sizeof(VECTOR3D), this->normals);
        glDrawElements(GL_QUADS, num_of_indices, GL_UNSIGNED_INT, this->indices);
        
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    
    glPopMatrix();
};

void ObjMesh :: setTextureMapID (int textureID) {
    this->textureID = textureID;
}

/*Loads the .obj file into the application*/
void load_obj (string filename, ObjMesh **mesh) {
    ifstream myFile;
    string line;
    string delimiter = "//";
    
    vector<VECTOR3D> * vertices = new vector<VECTOR3D>;
    vector<VECTOR3D> * normals = new vector<VECTOR3D>;;
    vector<GLuint> * indices = new vector<GLuint>;;
    vector<GLuint> * normal_indices = new vector<GLuint>;;
    
    myFile.open(filename);
    
    if (myFile.is_open()){
        while (getline(myFile, line)) {
            if (line.substr(0,2) == "v ") {
                istringstream s(line.substr(2));
                VECTOR3D v; s >> v.x; s >> v.y; s >> v.z; //pushes the vertcies into the vector
                vertices->push_back(v);
            }  else if (line.substr(0,2) == "f ") {
                istringstream s(line.substr(2));
                string token,token2; // stores the indice value
                GLushort a, b;
                string s2;
                
                for(int i = 0; i < 4;  i++) { // pushes the indicies of a face
                    s >> s2;
                    token = s2.substr(0, s2.find(delimiter));
                    token2 = s2.substr(s2.find(delimiter) + delimiter.length());
                    a = ::atof(token.c_str()); a--; // face indice
                    b = ::atof(token2.c_str()); b--; // normal indice
                    indices->push_back(a); normal_indices->push_back(b);
                }
            } else if (line.substr(0,2) == "vn") {
                istringstream s(line.substr(2));
                VECTOR3D vn; s >> vn.x; s >> vn.y; s >> vn.z; //pushes the vertex normals into the vector
                normals->push_back(vn);
            }
            else if (line[0] == '#') { /* ignoring this line */ }
            else { /* ignoring this line */ }
        }
        myFile.close();
    } else {
        printf("Unable to open %s\n",filename.c_str());
    }
    
    *mesh = new ObjMesh(*vertices,*normals,*indices, *normal_indices);
};