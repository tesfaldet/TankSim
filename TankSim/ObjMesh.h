#ifndef __CityBuilder2__ObjMesh__
#define __CityBuilder2__ObjMesh__

class Point3D {
    public:
    
    //Variables
    GLfloat x,y,z;
    
    //functions
    Point3D(void) : x(0.0), y(0.0), z(0.0){};
    Point3D(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {};
};

class ObjMesh {
public:
    Point3D *vertices = NULL;
    Point3D *normals = NULL;
    GLuint *indices = NULL; // unsigned integer
    GLuint *normal_indices = NULL;
    
    int num_of_vertices;
    int num_of_normals;
    int num_of_indices;
    
    //Transformations 
    float tx, ty, tz;			// Translatation Deltas
    float sfx, sfy, sfz;		// Scale Factors
    float angle;
    
    float mat_ambient[4];
    float mat_specular[4];
    float mat_diffuse[4];
    float mat_shininess[1];
    
    bool immediate_render = true;
    
    //Texturing
    int textureID = -1;
    
    //Functions
    ObjMesh (std::vector<Point3D> &vertices,std::vector<Point3D> &normals,std::vector<GLuint> &indices, std::vector<GLuint> &normal_indices);
    void draw ();
    void setTextureMapID (int textureID);
};


void load_obj (std::string filename, ObjMesh **mesh);

#endif /* defined(__CityBuilder2__ObjMesh__) */
