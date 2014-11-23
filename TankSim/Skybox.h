//
//  Skybox.h
//  TankSim
//
//  Created by Matthew Tesfaldet on 2014-11-23.
//  Copyright (c) 2014 Matthew Tesfaldet. All rights reserved.
//

#ifndef TankSim_Skybox_h
#define TankSim_Skybox_h

class Skybox {
private:
  int textureID[6] = {-1, -1, -1, -1, -1, -1};
  RGBpixmap skybox_pixelMap[6];
  char skybox_fileName[6][100] =
  {
    "textures/sky1.bmp",
    "textures/sky2.bmp",
    "textures/sky3.bmp",
    "textures/sky4.bmp",
    "textures/sky5.bmp",
    "textures/sky6.bmp",
  };
  
public:
  Skybox() {};
  
  Skybox(int textureID[6], const char** fileName) {
    this->textureID[0] = textureID[0];
    this->textureID[1] = textureID[1];
    this->textureID[2] = textureID[2];
    this->textureID[3] = textureID[3];
    this->textureID[4] = textureID[4];
    this->textureID[5] = textureID[5];
    
    strncpy(skybox_fileName[0], fileName[0], 100);
    skybox_fileName[0][99] = '\0';
    strncpy(skybox_fileName[1], fileName[1], 100);
    skybox_fileName[1][99] = '\0';
    strncpy(skybox_fileName[2], fileName[2], 100);
    skybox_fileName[2][99] = '\0';
    strncpy(skybox_fileName[3], fileName[3], 100);
    skybox_fileName[3][99] = '\0';
    strncpy(skybox_fileName[4], fileName[4], 100);
    skybox_fileName[4][99] = '\0';
    strncpy(skybox_fileName[5], fileName[5], 100);
    skybox_fileName[5][99] = '\0';
  };
  
  void setTextures() {
    readBMPFile(&skybox_pixelMap[0], skybox_fileName[0]);
    setTexture(&skybox_pixelMap[0], textureID[0]);
    readBMPFile(&skybox_pixelMap[1], skybox_fileName[1]);
    setTexture(&skybox_pixelMap[1], textureID[1]);
    readBMPFile(&skybox_pixelMap[2], skybox_fileName[2]);
    setTexture(&skybox_pixelMap[2], textureID[2]);
    readBMPFile(&skybox_pixelMap[3], skybox_fileName[3]);
    setTexture(&skybox_pixelMap[3], textureID[3]);
    readBMPFile(&skybox_pixelMap[4], skybox_fileName[4]);
    setTexture(&skybox_pixelMap[4], textureID[4]);
    readBMPFile(&skybox_pixelMap[5], skybox_fileName[5]);
    setTexture(&skybox_pixelMap[5], textureID[5]);
  }
    
  void DrawSkybox(float lookFromx, float lookFromy, float lookFromz, float upx, float upy, float upz) {
    // Store the current matrix
    glPushMatrix();
    // Reset and transform the matrix.
    glLoadIdentity();
    gluLookAt(0,0,0,
              lookFromx, -lookFromy, lookFromz,
              upx, upy, upz);
    // Enable/Disable features
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    // Just in case we set all vertices to white.
    glColor4f(1,1,1,1);
    // Render the front quad
    glBindTexture(GL_TEXTURE_2D, textureID[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
    glTexCoord2f(1, 0); glVertex3f( -0.5f, -0.5f, -0.5f );
    glTexCoord2f(1, 1); glVertex3f( -0.5f,  0.5f, -0.5f );
    glTexCoord2f(0, 1); glVertex3f(  0.5f,  0.5f, -0.5f );
    glEnd();
    // Render the left quad
    glBindTexture(GL_TEXTURE_2D, textureID[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(  0.5f, -0.5f,  0.5f );
    glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
    glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f, -0.5f );
    glTexCoord2f(0, 1); glVertex3f(  0.5f,  0.5f,  0.5f );
    glEnd();
    // Render the back quad
    glBindTexture(GL_TEXTURE_2D, textureID[2]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f,  0.5f );
    glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f,  0.5f );
    glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f,  0.5f );
    glTexCoord2f(0, 1); glVertex3f( -0.5f,  0.5f,  0.5f );
    glEnd();
    // Render the right quad
    glBindTexture(GL_TEXTURE_2D, textureID[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f, -0.5f );
    glTexCoord2f(1, 0); glVertex3f( -0.5f, -0.5f,  0.5f );
    glTexCoord2f(1, 1); glVertex3f( -0.5f,  0.5f,  0.5f );
    glTexCoord2f(0, 1); glVertex3f( -0.5f,  0.5f, -0.5f );
    glEnd();
    // Render the top quad
    glBindTexture(GL_TEXTURE_2D, textureID[4]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex3f( -0.5f,  0.5f, -0.5f );
    glTexCoord2f(0, 0); glVertex3f( -0.5f,  0.5f,  0.5f );
    glTexCoord2f(1, 0); glVertex3f(  0.5f,  0.5f,  0.5f );
    glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f, -0.5f );
    glEnd();
    // Render the bottom quad
    glBindTexture(GL_TEXTURE_2D, textureID[5]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f, -0.5f );
    glTexCoord2f(0, 1); glVertex3f( -0.5f, -0.5f,  0.5f );
    glTexCoord2f(1, 1); glVertex3f(  0.5f, -0.5f,  0.5f );
    glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
    glEnd();
    // Restore enable bits and matrix
    glPopAttrib();
    glPopMatrix();
  };
  
  void setTextureID(int ID[6]) {
      textureID[0] = ID[0];
      textureID[1] = ID[1];
      textureID[2] = ID[2];
      textureID[3] = ID[3];
      textureID[4] = ID[4];
      textureID[5] = ID[5];
  };
  
  void setFileName(const char** fileName) {
    strncpy(skybox_fileName[0], fileName[0], 100);
    skybox_fileName[0][99] = '\0';
    strncpy(skybox_fileName[1], fileName[1], 100);
    skybox_fileName[1][99] = '\0';
    strncpy(skybox_fileName[2], fileName[2], 100);
    skybox_fileName[2][99] = '\0';
    strncpy(skybox_fileName[3], fileName[3], 100);
    skybox_fileName[3][99] = '\0';
    strncpy(skybox_fileName[4], fileName[4], 100);
    skybox_fileName[4][99] = '\0';
    strncpy(skybox_fileName[5], fileName[5], 100);
    skybox_fileName[5][99] = '\0';
  }
};

#endif
