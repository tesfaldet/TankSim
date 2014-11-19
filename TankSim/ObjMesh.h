#ifndef __CityBuilder2__ObjMesh__
#define __CityBuilder2__ObjMesh__

class Point2D {
public:
    //constructors
    Point2D(void)	:	x(0.0f), y(0.0f)
    {}
    
    Point2D(float newX, float newY):x(newX), y(newY)
    {}
    
    float x, y;
};

class ObjMesh {
public:
    VECTOR3D *vertices = NULL;
    VECTOR3D *normals = NULL;
    Point2D *texture_coordinates = NULL;
    
    //Indicies
    GLuint *indices = NULL; // unsigned integer
    GLuint *normal_indices = NULL;
    GLuint *texture_indices = NULL;
    
    
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
    
    VECTOR3D center;
    bool use_center_x_translate = false; //test
    
    //Functions
    ObjMesh (std::vector<VECTOR3D> &vertices,std::vector<VECTOR3D> &normals,std::vector<GLuint> &indices, std::vector<GLuint> &normal_indices);
    void draw ();
    void setTextureMapID (int textureID);
    void setTextuteCoordinates(std::vector<Point2D> &tex_cord, std::vector<GLuint> &indicies);
};


void load_obj (std::string filename, ObjMesh **mesh);

#endif /* defined(__CityBuilder2__ObjMesh__) */
