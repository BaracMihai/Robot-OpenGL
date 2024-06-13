
#include "glos.h"

#include "gl.h"
#include "glu.h"
#include "glaux.h"
#include <math.h>

GLfloat ctrlpoints[3][3] = { {-4.0, 0.0, 0.0}, { 0.0, 15.0, 0.0}, { 4.0, 15.0, 0.0} };
static GLfloat x = 0;
GLuint textureId1;
static int alfaX = 0, alfaY = 0, alfaZ = 0, speed = 0;

GLubyte rasters[][13] = {

{0x00, 0x00, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18},
{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18},
{0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18},
{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e},
{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e},
{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},
{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xff, 0xff, 0xe7, 0xc3},
{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e},
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18},
{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e},
};

GLuint fontOffset;

void makeRasterFont()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    fontOffset = glGenLists(128);
    for (int i = 32; i < 64; i++) {
        glNewList(i + fontOffset, GL_COMPILE);
        {
            glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, rasters[i - 32]);
        }
        glEndList();
    }
}

void printString(const char* s)
{
    glPushAttrib(GL_LIST_BIT);
    glListBase(fontOffset);
    glCallLists(strlen(s), GL_UNSIGNED_BYTE, (GLubyte*)s);
    glPopAttrib();
}


GLuint incarcaTextura(const char* s)
{
    GLuint textureId = 0;
    AUX_RGBImageRec* pImagineTextura = auxDIBImageLoad(s);

    if (pImagineTextura != NULL)
    {
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, pImagineTextura->sizeX, pImagineTextura->sizeY,
            0, GL_RGB, GL_UNSIGNED_BYTE, pImagineTextura->data);
    }
    if (pImagineTextura)
    {
        if (pImagineTextura->data) {
            free(pImagineTextura->data);
        }
        free(pImagineTextura);
    }
    return textureId;
}

void myInit() {
    glClearColor(0.0f, 0.7f, 0.8f, 1.0f);
    makeRasterFont();
    glEnable(GL_MAP1_VERTEX_3);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    
    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_specular[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat light_position0[] = { 0.0, 0.0, 2.0, 0.0 };
    GLfloat light_position1[] = { 2.0, 0.0, -2.0, 0.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

    glEnable(GL_TEXTURE_2D);

}

void CALLBACK MutaStanga()
{
    x = x - 10;
}

void CALLBACK MutaDreapta()
{
    x = x + 10;
}

void CALLBACK Slow()
{
    if (alfaX < 28)
        alfaX = (alfaX + 3) % 360;
    speed -= 5;
}

void CALLBACK Speed()
{
    if(alfaX > -28)
        alfaX = (alfaX - 3) % 360;
    speed += 5;
}

void CALLBACK RotateYRight()
{
    alfaY = (alfaY + 5) % 360;
}

void CALLBACK RotateYLeft()
{
    alfaY = (alfaY - 5) % 360;
}

void CALLBACK RotateZUp()
{
    alfaZ = (alfaZ + 5) % 360;
}

void CALLBACK RotateZDown()
{
    alfaZ = (alfaZ - 5) % 360;
}


void CALLBACK display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    char teststring[33];

    for (int i = 32; i < 64; i += 32)
    {
        glRasterPos2i(50, -130);
        for (int j = 0; j < 32; j++)
            teststring[j] = (char)(i + j);

        teststring[32] = 0;
        printString(teststring);
    }

    glTranslatef(x, 0, 0.0);
    glRotatef(alfaY, 0.0, 1.0, 0.0);
    glRotatef(alfaZ, 0.0, 0.0, 1.0);

    
    glTranslatef(0.0, -105.0, 0.0);
    glRotatef(alfaX, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 105.0, 0.0);
    
    GLfloat metalizat_gri_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat metalizat_gri_specular[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat metalizat_gri_shininess[] = { 90.0f };

    GLfloat metalizat_verde_diffuse[] = { 0.4f, 1.0f, 0.7f, 1.0f };
    GLfloat metalizat_verde_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat metalizat_verde_shininess[] = { 90.0f };

    GLfloat metalizat_rosu_diffuse[] = { 1.0f, 0.2f, 0.2f, 1.0f };
    GLfloat metalizat_rosu_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat metalizat_rosu_shininess[] = { 90.0f };

    GLfloat mat_negru_diffuse[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat mat_negru_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat mat_negru_shininess[] = { 10.0f };

    GLfloat metalizat_albastru_diffuse[] = { 0.1f, 0.5f, 1.0f, 1.0f };
    GLfloat metalizat_albastru_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat metalizat_albastru_shininess[] = { 70.0f };

    glMaterialfv(GL_FRONT, GL_DIFFUSE, metalizat_gri_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, metalizat_gri_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, metalizat_gri_shininess);
    auxSolidBox(50.0, 100.0, 50.0); 

    glMaterialfv(GL_FRONT, GL_DIFFUSE, metalizat_verde_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, metalizat_verde_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, metalizat_verde_shininess);
    glTranslatef(30.0, 30.0, 0.0);
    glRotatef(-alfaX, 1.0, 0.0, 0.0); 
    auxSolidSphere(10.0); 

    glMaterialfv(GL_FRONT, GL_DIFFUSE, metalizat_gri_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, metalizat_gri_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, metalizat_gri_shininess);
    glTranslatef(0.0, -10.0, 0.0);
    auxSolidCylinder(4.0, 30.0); 

    glMaterialfv(GL_FRONT, GL_DIFFUSE, metalizat_verde_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, metalizat_verde_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, metalizat_verde_shininess);
    glTranslatef(0.0, -28.0, 0.0);
    auxSolidSphere(5.0); 

    glMaterialfv(GL_FRONT, GL_DIFFUSE, metalizat_gri_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, metalizat_gri_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, metalizat_gri_shininess);
    glTranslatef(0.0, -4.0, 0.0);
    auxSolidCylinder(3.8, 30.0); 

    glMaterialfv(GL_FRONT, GL_DIFFUSE, metalizat_rosu_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, metalizat_rosu_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, metalizat_rosu_shininess);
    glTranslatef(0.0, -28.0, 0.0);
    auxSolidIcosahedron(7.0); 
    glTranslatef(0.0, 70.0, 0.0);
    glRotatef(alfaX, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -70.0, 0.0);

    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, metalizat_verde_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, metalizat_verde_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, metalizat_verde_shininess);
    glTranslatef(-60.0, 70.0, 0.0);
    glRotatef(-alfaX, 1.0, 0.0, 0.0); 
    auxSolidSphere(10.0); 

    glMaterialfv(GL_FRONT, GL_DIFFUSE, metalizat_gri_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, metalizat_gri_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, metalizat_gri_shininess);
    glTranslatef(0.0, -10.0, 0.0);
    auxSolidCylinder(4.0, 30.0); 

    glMaterialfv(GL_FRONT, GL_DIFFUSE, metalizat_verde_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, metalizat_verde_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, metalizat_verde_shininess);
    glTranslatef(0.0, -28.0, 0.0);
    auxSolidSphere(5.0); 

    glMaterialfv(GL_FRONT, GL_DIFFUSE, metalizat_gri_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, metalizat_gri_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, metalizat_gri_shininess);
    glTranslatef(0.0, -4.0, 0.0);
    auxSolidCylinder(3.8, 30.0);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, metalizat_rosu_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, metalizat_rosu_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, metalizat_rosu_shininess);
    glTranslatef(0.0, -28.0, 0.0);
    auxSolidIcosahedron(7.0); 
    glTranslatef(0.0, 70.0, 0.0);
    glRotatef(alfaX, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -70.0, 0.0);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, metalizat_gri_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, metalizat_gri_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, metalizat_gri_shininess);
    glTranslatef(30.0, 100.0, 0.0);
    auxSolidCylinder(10, 15); 

    glMaterialfv(GL_FRONT, GL_DIFFUSE, metalizat_gri_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, metalizat_gri_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, metalizat_gri_shininess);
    glTranslatef(0.0, 20.0, 0.0);
    auxSolidCube(40.0); 
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_negru_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_negru_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_negru_shininess);
    glTranslatef(0.0, 20.0, 0.0);
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 3, &ctrlpoints[0][0]);
    glBegin(GL_LINE_STRIP); 
    {
        for (int i = 0; i <= 30; i++) {
            glEvalCoord1f((GLfloat)i / 30.0f); 
        }
    }
    glEnd();
    
    glTranslatef(4.0, 15.0, 0.0);
    if (sqrt((alfaY % 45) * (alfaY % 45)) < 22.5)
    {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_negru_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_negru_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_negru_shininess);
    }
    else
    {   
        glMaterialfv(GL_FRONT, GL_DIFFUSE, metalizat_rosu_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, metalizat_rosu_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, metalizat_rosu_shininess);
    }
    auxSolidSphere(3.0); 

    glMaterialfv(GL_FRONT, GL_DIFFUSE, metalizat_gri_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, metalizat_gri_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, metalizat_gri_shininess);
    glTranslatef(-3.0, -160.0, 0.0);
    auxSolidCylinder(5, 40); 


    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_negru_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_negru_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_negru_shininess);
    glTranslatef(0.0, -60.0, 0.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    glRotatef(speed, 0.0, 0.0, 1.0);
    auxSolidTorus(10,20); 

    glMaterialfv(GL_FRONT, GL_DIFFUSE, metalizat_gri_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, metalizat_gri_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, metalizat_gri_shininess);
    auxSolidTorus(1, 10);
    for (int i = 0; i < 4; i++) {
        glRotatef(45, 0.0, 0.0, 1.0);
        auxSolidBox(20.0, 1.0, 1.0); 
    }
    glRotatef(-speed, 0.0, 0.0, 1.0);
    glRotatef(180, 0.0, 0.0, 1.0);
    
    glTranslatef(0.0, 185.0, 15.0);
    auxSolidSphere(12.0); 
    glTranslatef(0.0, 0.0, -33.0);
    auxSolidSphere(12.0); 

    glMaterialfv(GL_FRONT, GL_DIFFUSE, metalizat_albastru_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, metalizat_albastru_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, metalizat_albastru_shininess);
    glTranslatef(-20.0, 8.0, 10.0);
    auxSolidSphere(5.0); 
    glTranslatef(0.0, 0.0, 14.0); 
    auxSolidSphere(5.0);


    glMaterialfv(GL_FRONT, GL_DIFFUSE, metalizat_rosu_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, metalizat_rosu_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, metalizat_rosu_shininess);
    glTranslatef(7.0, -15.0, 2.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    auxSolidCylinder(10.0, 20.0); 
    
    

    glFlush();
}

void CALLBACK IdleFunction() {
    alfaY++;
    if(alfaX >= -30 && alfaX < 0)
        alfaX += 0.2;
    if (alfaX <= 30 && alfaX > 0)
        alfaX -= 0.2;
    display();
    Sleep(1000.0 / 60.0);
}

#define ISOTROPIC
#ifdef ISOTROPIC
void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) {
        return;
    }
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h) {
        glOrtho(-160.0, 160.0, 160.0 * (GLfloat)h / (GLfloat)w, -160.0 * (GLfloat)h / (GLfloat)w, -250.0, 200.0);
    }
    else {
        glOrtho(-160.0 * (GLfloat)w / (GLfloat)h, 160.0 * (GLfloat)w / (GLfloat)h, -160.0, 160.0, -250.0, 200.0);
    }
    glMatrixMode(GL_MODELVIEW);
}
#else
void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) return;			
    glViewport(0, 0, w, h);	
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-160.0, 160.0, -160.0, 160.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}
#endif

int main(int argc, char** argv)
{
    auxInitDisplayMode(AUX_SINGLE | AUX_RGB);
    auxInitPosition(0, 0, 800, 600);
    auxInitWindow("Un robot");
    auxIdleFunc(IdleFunction);
    myInit();
    auxKeyFunc(AUX_LEFT, MutaStanga);
    auxKeyFunc(AUX_RIGHT, MutaDreapta);
    auxKeyFunc(AUX_a, RotateYLeft);
    auxKeyFunc(AUX_d, RotateYRight);
    auxKeyFunc(AUX_s, RotateZDown);
    auxKeyFunc(AUX_w, RotateZUp);
    auxKeyFunc(AUX_e, Speed);
    auxKeyFunc(AUX_q, Slow);

    auxReshapeFunc(myReshape);
    auxMainLoop(display);
    return(0);
}

