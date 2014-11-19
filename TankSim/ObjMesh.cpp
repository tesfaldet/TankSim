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
    
    for(int i =0; i < this->num_of_vertices; i++){
        this->center += vertices[i];
    }
    
    this->center.x = this->center.x / num_of_vertices;
    this->center.y = this->center.y / num_of_vertices;
    this->center.z = this->center.z / num_of_vertices;
};

void ObjMesh :: setTextuteCoordinates(std::vector<Point2D> &tex_cord, std::vector<GLuint> &indicies) {
    this->texture_coordinates = &tex_cord[0];
    this->texture_indices = &indicies[0];
}

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
    if (use_center_x_translate) {
       glTranslatef(center.x, center.y, center.z);
       glRotatef(this->angles.x, 1, 0, 0);
       glTranslatef(-center.x, -center.y, -center.z);
    } else {
       glRotatef(this->angles.x, 1, 0, 0);
    }
    
    glRotatef(this->angles.z, 0, 0, 1);
    glScalef(this->scaleFactor.x, this->scaleFactor.y, this->scaleFactor.z);
    
    //Rendering
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glBegin(GL_TRIANGLES);
    glEnable(GL_BLEND);
    
    if (this->texture_coordinates) {
        for(int i = 0; i < num_of_indices; i +=3) {
            
            glNormal3f(this->normals[this->normal_indices[i]].x,
                       this->normals[this->normal_indices[i]].y,
                       this->normals[this->normal_indices[i]].z);
        
            glTexCoord2f(this->texture_coordinates[this->texture_indices[i]].x,
                         this->texture_coordinates[this->texture_indices[i]].y);
            glVertex3f(this->vertices[this->indices[i]].x,
                       this->vertices[this->indices[i]].y,
                       this->vertices[this->indices[i]].z);
            glTexCoord2f(this->texture_coordinates[this->texture_indices[i+1]].x,
                         this->texture_coordinates[this->texture_indices[i+1]].y);
            
            glVertex3f(this->vertices[this->indices[i+1]].x,
                       this->vertices[this->indices[i+1]].y,
                       this->vertices[this->indices[i+1]].z);
            
            glTexCoord2f(this->texture_coordinates[this->texture_indices[i+2]].x,
                         this->texture_coordinates[this->texture_indices[i+2]].y);
            glVertex3f(this->vertices[this->indices[i+2]].x,
                       this->vertices[this->indices[i+2]].y,
                       this->vertices[this->indices[i+2]].z);
        }
    } else {
        for(int i = 0; i < num_of_indices; i +=3) {
            
            glNormal3f(this->normals[this->normal_indices[i]].x,
                       this->normals[this->normal_indices[i]].y,
                       this->normals[this->normal_indices[i]].z);
            
            glVertex3f(this->vertices[this->indices[i]].x,
                       this->vertices[this->indices[i]].y,
                       this->vertices[this->indices[i]].z);
            
            glVertex3f(this->vertices[this->indices[i+1]].x,
                       this->vertices[this->indices[i+1]].y,
                       this->vertices[this->indices[i+1]].z);
            
            glVertex3f(this->vertices[this->indices[i+2]].x,
                       this->vertices[this->indices[i+2]].y,
                       this->vertices[this->indices[i+2]].z);
        }
    }
    
    glDisable(GL_BLEND);
    glEnd();
    
    glPopMatrix();
};

void ObjMesh :: setTextureMapID (int textureID) {
    this->textureID = textureID;
}

/*Loads the .obj file into the application
 requires all faces to be Trianles
 Texture cordinates 
 normal cordinates
 */
void load_obj (string filename, ObjMesh **mesh) {
    ifstream myFile;
    string line;
    string delimiter = "/";
    string delimiter2 = "//";
    
    vector<VECTOR3D> * vertices = new vector<VECTOR3D>;
    vector<VECTOR3D> * normals = new vector<VECTOR3D>;
    vector<Point2D> * tex_cord = new vector<Point2D>;
    vector<GLuint> * indices = new vector<GLuint>;
    vector<GLuint> * normal_indices = new vector<GLuint>;
    vector<GLuint> * tex_indices = new vector<GLuint>;
    
    bool is_text_cord = false; //used to load texture cordinates
    
    myFile.open(filename);
    
    if (myFile.is_open()){
        while (getline(myFile, line)) {
            if (line.substr(0,2) == "v ") {
                istringstream s(line.substr(2));
                VECTOR3D v; s >> v.x; s >> v.y; s >> v.z; //pushes the vertcies into the vector
                vertices->push_back(v);
            }  else if (line.substr(0,2) == "f ") {
                istringstream s(line.substr(2));
                string token; // stores the indice value
                GLushort a, b, c;
                string s2;
                
                if (is_text_cord) {
                    for(int i = 0; i < 3;  i++) { // pushes the indicies of a face
                        s >> s2;
                        //Face
                        token = s2.substr(0, s2.find(delimiter));
                        a = ::atof(token.c_str()); a--; // face indice
                        s2.erase(0, s2.find(delimiter) + delimiter.length());
                        indices->push_back(a);
                    
                        //UV Indice
                        token = s2.substr(0, s2.find(delimiter));
                        b = ::atof(token.c_str()); b--; // texture indice
                        s2.erase(0, s2.find(delimiter) + delimiter.length());
                        tex_indices->push_back(b);
                        
                        //Normal Indice
                        token = s2.substr(0, s2.find(delimiter));
                        c = ::atof(token.c_str()); c--; // normal indice
                        normal_indices->push_back(c);
                    }
                } else {
                    for(int i = 0; i < 3;  i++) { // pushes the indicies of a face
                        s >> s2;
                        //Face
                        token = s2.substr(0, s2.find(delimiter2));
                        a = ::atof(token.c_str()); a--; // face indice
                        s2.erase(0, s2.find(delimiter2) + delimiter2.length());
                        indices->push_back(a);
                        
                        //Normal Indice
                        token = s2.substr(0, s2.find(delimiter2));
                        c = ::atof(token.c_str()); c--; // normal indice
                        normal_indices->push_back(c);
                    }
                }
                
            } else if (line.substr(0,2) == "vn") {
                istringstream s(line.substr(2));
                VECTOR3D vn; s >> vn.x; s >> vn.y; s >> vn.z; //pushes the vertex normals into the vector
                normals->push_back(vn);
                
            } else if (line.substr(0,2) == "vt") {
                is_text_cord = true;
                istringstream s(line.substr(2));
                Point2D uv; s >> uv.x; s >> uv.y; //pushes the vertex normals into the vector
                tex_cord->push_back(uv);
            }
            
            else if (line[0] == '#') { /* ignoring this line */ }
            else { /* ignoring this line */ }
        }
        myFile.close();
    } else {
        printf("Unable to open %s\n",filename.c_str());
    }
    
    (*mesh) = new ObjMesh(*vertices,*normals,*indices, *normal_indices);
    
    if (is_text_cord) {
        (*mesh)->setTextuteCoordinates(*tex_cord,*tex_indices);
    }
};