#ifndef __CityBuilder2__ObjMesh__
#define __CityBuilder2__ObjMesh__

class ObjMesh {
public:
    VECTOR3D *vertices = NULL;
    VECTOR3D *normals = NULL;
    GLuint *indices = NULL; // unsigned integer
    GLuint *normal_indices = NULL;
    
    int num_of_vertices;
    int num_of_normals;
    int num_of_indices;
    
    //Transformations 
    VECTOR3D translation;		// Translatation Deltas
    VECTOR3D scaleFactor;		// X, Y, Z Scale Factors
    VECTOR3D angles;            // Rotation angles around x, y, z axis
    
    //Material
    float mat_ambient[4];
    float mat_specular[4];
    float mat_diffuse[4];
    float mat_shininess[1];
    
    //Texturing
    int textureID = -1;
    
    //Rendering Mode
    bool immediate_render = true;
    
    //Functions
    ObjMesh (std::vector<VECTOR3D> &vertices,std::vector<VECTOR3D> &normals,std::vector<GLuint> &indices, std::vector<GLuint> &normal_indices);
    void draw ();
    void setTextureMapID (int textureID);
};


void load_obj (std::string filename, ObjMesh **mesh);

#endif /* defined(__CityBuilder2__ObjMesh__) */
