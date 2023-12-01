#include <GL/glut.h>
#include <stdio.h>
#include <mmsystem.h>

GLfloat angulo=0.0;

GLfloat angX=0.0,angY=0.0,angZ=0.0,mod_angX=0.0,mod_angY=0.0,mod_angZ=0.0;
GLfloat mod_angXPelota = 0.0, mod_angYPelota = 0.0, posicion_x = 0.0, posicion_y = 0.0;

typedef struct{
	GLubyte *dibujo;//puntero de nuestra textura
	GLuint bpp;//bits por punto
	GLuint largo;
	GLuint ancho;
	GLuint ID;
}textura;

textura TexCaja;
textura TexNube;
textura TexFondo;
textura TexFondoVerde;
textura TexLetra_Mario;
textura TexLetra_Moneda;
textura TexLadrillo;
textura TexTubo;
textura TexEstrella;
textura TexYoshi;
textura TexMoneda;
textura TexFantasma;
textura TexCaraHongo;
textura TexCabezaHongo;
textura TexCaraEsfera;
textura TexPlantaCarnivora;

int cargaTGA(char *nombre, textura *imagen){
  GLubyte cabeceraTGA[12]={0,0,2,0,0,0,0,0,0,0,0,0};
  GLubyte compararTGA[12];//0,0,2,0,0,0,0,0,0,0,0
  GLubyte cabecera[6];//244,1,244,1,32,40
  GLuint bytesporpuntos;
  GLuint tamanoimagen;
  GLuint temp,i;
  GLuint tipo=GL_RGBA;
  
	FILE *archivo=fopen(nombre,"rb");
	
	if(
	archivo==NULL ||
	fread(compararTGA,1,sizeof(compararTGA),archivo) !=sizeof(compararTGA) ||
	memcmp(cabeceraTGA,compararTGA,sizeof(compararTGA)) != 0 ||
	fread(cabecera,1,sizeof(cabecera),archivo) != sizeof(cabecera)
	
	){
	
	   if(archivo==NULL){
	   	printf("No se encontro archivo %s\n",nombre);
	   	return 0;
	   }else {
	   	   fclose(archivo);
	   	   return 0;
	   }
		
	}
	
	imagen->largo =256 *cabecera[1] +cabecera[0];//256*1  +244=256+244=500
	imagen->ancho =256 * cabecera[3]+ cabecera[2];//256*1 + 244=256+244=500
	if(
	imagen->largo <=0 ||
	imagen->ancho <=0 ||
	(cabecera[4] !=24 && cabecera[4] !=32)
	)
	{
		printf("Datos invalidos\n");
		fclose(archivo);
		return 0;
	}
	imagen->bpp=cabecera[4];
	bytesporpuntos=cabecera[4]/8;
	tamanoimagen=imagen->largo * imagen->ancho* bytesporpuntos;//500*500*4=1000000
	// 1000000/1024=976,56=977 kb
	
	imagen->dibujo=(GLubyte *)malloc(tamanoimagen);
	
	if(
	imagen->dibujo==NULL ||
	fread(imagen->dibujo,1,tamanoimagen,archivo)!= tamanoimagen
	){
		if(imagen->dibujo !=NULL){
			printf("Error leyendo imagen\n");
			free(imagen->dibujo);
	}else {
		printf("Error asignando memoria\n");
	}
	
	fclose(archivo);
	return 0;
	}
	//TGA:BGRA          RGBA
	for(i=0;i<(int)tamanoimagen;i+=bytesporpuntos){
	   temp=imagen->dibujo[i];
	   imagen->dibujo[i]= imagen->dibujo[i+2];
	   imagen->dibujo[i+2]=temp;
	   	
	}
	fclose(archivo);
	
	
	glGenTextures(1,&imagen[0].ID);
	glBindTexture(GL_TEXTURE_2D,imagen[0].ID);
	
	if(imagen->bpp==24) tipo=GL_RGB;
	
	glTexImage2D(GL_TEXTURE_2D,0,tipo,imagen[0].ancho,imagen[0].largo,0,tipo,GL_UNSIGNED_BYTE,imagen[0].dibujo);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    return 1;
}


void fondo() {
    glDisable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, TexFondo.ID);
    glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);
	int c=700;    
    glBegin(GL_QUADS);
        glTexCoord2f(0.0,0.0);        glVertex3d(-c,c,0);
        glTexCoord2f(1.0,0.0);        glVertex3d(c,c,0);
        glTexCoord2f(1.0,1.0);        glVertex3d(c,-c,0);
        glTexCoord2f(0.0,1.0);        glVertex3d(-c,-c,0);
    glEnd();
    
    glEnable(GL_BLEND);  // Habilitar mezcla alfa
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBindTexture(GL_TEXTURE_2D, TexFondoVerde.ID);  // Cambiar a la textura del fondo verde
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3d(-c, c, 0);
        glTexCoord2f(1.0, 0.0);
        glVertex3d(c, c, 0);
        glTexCoord2f(1.0, 1.0);
        glVertex3d(c, -c, 0);
        glTexCoord2f(0.0, 1.0);
        glVertex3d(-c, -c, 0);
    glEnd();
    glLoadIdentity();
    glEnable(GL_LIGHTING);
    
}
void Letra(textura tex,int x ,int y ,int z) {
    glDisable(GL_LIGHTING);    
    glBindTexture(GL_TEXTURE_2D, tex.ID);
    glEnable(GL_TEXTURE_2D);
	//glColor3f(0.4431, 0.7961, 0.9333);
	glTranslatef(x, y, z);
	int c=60;    
    glBegin(GL_QUADS);
        glTexCoord2f(0.0,0.0);
        glVertex3d(-c,c,0);
        glTexCoord2f(1.0,0.0);
        glVertex3d(c,c,0);
        glTexCoord2f(1.0,1.0);
        glVertex3d(c,-c,0);
        glTexCoord2f(0.0,1.0);
        glVertex3d(-c,-c,0);
    glEnd();
    glLoadIdentity();
    glEnable(GL_LIGHTING);
    
}

void init(){
	GLfloat light_position_obj1[] = {-1,1,1,0};
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_POSITION,light_position_obj1);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glShadeModel(GL_SMOOTH);//MEJORA EL DIBUJO DE LA SOMBRA DE LA FIG
	glClearColor(0.0,0.0,0.0,0.0);
	glClearDepth(1.0);//borramos todo tipo de profundidad
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);//MEJORAMIENTO DE ENFOQUE Y FORMA DE VISUALIZAR EL GRAFICO
	glEnable(GL_TEXTURE_2D);
	//Carga de textura
	if(!cargaTGA("TexCaja.tga",&TexCaja)){
		printf("Error cargando tetxura\n");
		exit(0);
	}
	if(!cargaTGA("nube2.tga",&TexNube)){
		printf("Error cargando tetxura\n");
		exit(0);
	}
	if(!cargaTGA("fondo.tga",&TexFondo)){
		printf("Error cargando tetxura\n");
		exit(0);
	}
	if(!cargaTGA("fondoVerde.tga",&TexFondoVerde)){
		printf("Error cargando tetxura\n");
		exit(0);
	}
	if(!cargaTGA("letra_mario.tga",&TexLetra_Mario)){
		printf("Error cargando tetxura\n");
		exit(0);
	}
	if(!cargaTGA("letra_moneda.tga",&TexLetra_Moneda)){
		printf("Error cargando tetxura\n");
		exit(0);
	}
	if(!cargaTGA("TexCubo.tga",&TexLadrillo)){
		printf("Error cargando tetxura\n");
		exit(0);
	}
	if(!cargaTGA("tubo.tga",&TexTubo)){
		printf("Error cargando tetxura\n");
		exit(0);
	}
	if(!cargaTGA("estrella.tga",&TexEstrella)){
		printf("Error cargando tetxura\n");
		exit(0);
	}
	if(!cargaTGA("yoshi.tga",&TexYoshi)){
		printf("Error cargando tetxura\n");
		exit(0);
	}
	if(!cargaTGA("moneda.tga",&TexMoneda)){
		printf("Error cargando tetxura\n");
		exit(0);
	}
	if(!cargaTGA("ghost3.tga",&TexFantasma)){
		printf("Error cargando tetxura\n");
		exit(0);
	}
	if(!cargaTGA("cara_hongo.tga",&TexCaraHongo)){
		printf("Error cargando tetxura\n");
		exit(0);
	}
	if(!cargaTGA("cabeza_hongo.tga",&TexCabezaHongo)){
		printf("Error cargando tetxura\n");
		exit(0);
	}
	if(!cargaTGA("TexPersonaje.tga",&TexCaraEsfera)){
		printf("Error cargando tetxura\n");
		exit(0);
	}
	if(!cargaTGA("cabeza_planta.tga",&TexPlantaCarnivora)){
		printf("Error cargando tetxura\n");
		exit(0);
	}
}


void yoshi(textura tex,int ancho,int largo,float vel) {
    glDisable(GL_LIGHTING);    
    glBindTexture(GL_TEXTURE_2D, tex.ID);
    
    glEnable(GL_TEXTURE_2D);
	glTranslatef(220, -400, 10);
	glRotatef(angulo*vel,0,1,0);
	glTranslatef(-120, -20, 40); 
	int c=ancho ; 
	int d=largo;   
    glBegin(GL_QUADS);
        glTexCoord2f(0.0,0.0);
        glVertex3d(-c,d,0);
        glTexCoord2f(1.0,0.0);
        glVertex3d(c,d,0);
        glTexCoord2f(1.0,1.0);
        glVertex3d(c,-c,0);
        glTexCoord2f(0.0,1.0);
        glVertex3d(-c,-c,0);
    glEnd();
    glLoadIdentity();
    glEnable(GL_LIGHTING);
    
}

void Fig2d(textura tex,int x ,int y ,int z,int ancho,int largo,int vel) {
    glDisable(GL_LIGHTING);    
    glBindTexture(GL_TEXTURE_2D, tex.ID);
    
    glEnable(GL_TEXTURE_2D);
	glTranslatef(x, y, z);
	glRotatef(angulo*vel,0,1,0);
	int c=ancho ;
	int d=largo;
    glBegin(GL_QUADS);
        glTexCoord2f(0.0,0.0);
        glVertex3d(-c,d,0);
        glTexCoord2f(1.0,0.0);
        glVertex3d(c,d,0);
        glTexCoord2f(1.0,1.0);
        glVertex3d(c,-c,0);
        glTexCoord2f(0.0,1.0);
        glVertex3d(-c,-c,0);
    glEnd();
    glLoadIdentity();
    glEnable(GL_LIGHTING);
    
}
void aro_hongo(){
	glEnable(GL_LIGHTING);
    glPushMatrix();

    glTranslatef(1, 25, 0);
    glRotatef(90, 1, 0, 0);
   	
    GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_diffuse[] = { 0.8, 0.0, 0.0, 1.0 };
    GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 0.0 };
    GLfloat shininess[] = { 10.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    glutSolidTorus(10, 25, 30, 30);

    glPopMatrix();
    glDisable(GL_LIGHTING);
}
void Esfera(textura Tex,int tam ){
	
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	GLUquadric *qobj = gluNewQuadric();
	gluQuadricTexture(qobj,GL_TRUE);
	glBindTexture(GL_TEXTURE_2D,Tex.ID);
	gluSphere(qobj,tam,50,50);
	gluDeleteQuadric(qobj);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}
void ojo(float radius, float verticalScale) {
    glDisable(GL_LIGHTING);
    glPushAttrib(GL_CURRENT_BIT);
    glColor3f(0.0f, 0.0f, 0.0f); 
    glEnable(GL_TEXTURE_2D);
    GLUquadric *qobj = gluNewQuadric();
    gluQuadricTexture(qobj, GL_TRUE);
    glScalef(1.0, verticalScale, 1.0);
    gluSphere(qobj, radius, 50, 50);
    gluDeleteQuadric(qobj);
    glPopAttrib(); 
    glEnable(GL_LIGHTING);
}
void Hongo(){
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D,TexCaraHongo.ID);
	glTranslatef(120, 220,10);	
	int c=25;
	int d=23;
	glBegin(GL_QUADS);
	//frente
	  glTexCoord2f(0.0,0.0);glVertex3d(-c,d,c);
	  glTexCoord2f(1.0,0.0);glVertex3d(c,d,c);
	  glTexCoord2f(1.0,1.0);glVertex3d(c-5,-c,c);
	  glTexCoord2f(0.0,1.0);glVertex3d(-c+5,-c,c);
	//derecha
	  glTexCoord2f(0.0,0.0);glVertex3d(c,d,c);
	  glTexCoord2f(1.0,0.0);glVertex3d(c,d,-c);
	  glTexCoord2f(1.0,1.0);glVertex3d(c-5,-c,-c);
	  glTexCoord2f(0.0,1.0);glVertex3d(c-5,-c,c);
	//atras
	  glTexCoord2f(0.0,0.0);glVertex3d(c,d,-c);
	  glTexCoord2f(1.0,0.0);glVertex3d(-c,d,-c);
	  glTexCoord2f(1.0,1.0);glVertex3d(-c+5,-c,-c);
	  glTexCoord2f(0.0,1.0);glVertex3d(c-5,-c,-c);
	  //izquierda
	  glTexCoord2f(0.0,0.0); glVertex3d(-c,d,-c);
	  glTexCoord2f(1.0,0.0); glVertex3d(-c,d,c);
	  glTexCoord2f(1.0,1.0); glVertex3d(-c+5,-c,c);
	  glTexCoord2f(0.0,1.0); glVertex3d(-c+5,-c,-c);
	  //arriba
	  glTexCoord2f(0.0,0.0);glVertex3d(-c,c,-c);
	  glTexCoord2f(1.0,0.0);glVertex3d(c,c,-c);
	  glTexCoord2f(1.0,1.0);glVertex3d(c,c,c);
	  glTexCoord2f(0.0,1.0);glVertex3d(-c,c,c);
	  //abajo
	  glTexCoord2f(0.0,0.0);glVertex3d(-c,-c,c);
	  glTexCoord2f(1.0,0.0);glVertex3d(c,-c,c);
	  glTexCoord2f(1.0,1.0);glVertex3d(c,-c,-c);
	  glTexCoord2f(0.0,1.0);glVertex3d(-c,-c,-c);
	glEnd();
	
	glScalef(1.0,1.5,1.0);
	aro_hongo();
	
	glTranslated(0,40,0);
	glRotatef(90,1,1,0);
	Esfera(TexCabezaHongo,30);

	glLoadIdentity();	
	glEnable(GL_LIGHTING);
	}

void Fig3d(textura Text,int x,int y,int z,int ancho ,int largo,float vel){
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D,Text.ID);
	glTranslatef(x, y, z);
	glRotatef(angulo*vel,0,1,0);	
	int c =ancho;
	int d=largo;

	glBegin(GL_QUADS);

	  glTexCoord2f(0.0,0.0);glVertex3d(-c,d,c);
	  glTexCoord2f(1.0,0.0);glVertex3d(c,d,c);
	  glTexCoord2f(1.0,1.0);glVertex3d(c,-c,c);
	  glTexCoord2f(0.0,1.0);glVertex3d(-c,-c,c);

	  glTexCoord2f(0.0,0.0);glVertex3d(c,d,c);
	  glTexCoord2f(1.0,0.0);glVertex3d(c,d,-c);
	  glTexCoord2f(1.0,1.0);glVertex3d(c,-c,-c);
	  glTexCoord2f(0.0,1.0);glVertex3d(c,-c,c);

	  glTexCoord2f(0.0,0.0);glVertex3d(c,d,-c);
	  glTexCoord2f(1.0,0.0);glVertex3d(-c,d,-c);
	  glTexCoord2f(1.0,1.0);glVertex3d(-c,-c,-c);
	  glTexCoord2f(0.0,1.0);glVertex3d(c,-c,-c);
	  
	  glTexCoord2f(0.0,0.0); glVertex3d(-c,d,-c);
	  glTexCoord2f(1.0,0.0); glVertex3d(-c,d,c);
	  glTexCoord2f(1.0,1.0); glVertex3d(-c,-c,c);
	  glTexCoord2f(0.0,1.0); glVertex3d(-c,-c,-c);
	  
	  
	  glTexCoord2f(0.0,0.0);glVertex3d(-c,d,-c);
	  glTexCoord2f(1.0,0.0);glVertex3d(c,d,-c);
	  glTexCoord2f(1.0,1.0);glVertex3d(c,d,c);
	  glTexCoord2f(0.0,1.0);glVertex3d(-c,d ,c);
	  
	  glTexCoord2f(0.0,0.0);glVertex3d(-c,-c,c);
	  glTexCoord2f(1.0,0.0);glVertex3d(c,-c,c);
	  glTexCoord2f(1.0,1.0);glVertex3d(c,-c,-c);
	  glTexCoord2f(0.0,1.0);glVertex3d(-c,-c,-c);

	glEnd();
	glLoadIdentity();
	glEnable(GL_LIGHTING);
}

void animacionPelota(){
	posicion_x+=mod_angXPelota;
	posicion_y+=mod_angYPelota;	
	if(posicion_x>=360) posicion_x=0;
	if(posicion_y>=360) posicion_y=0;
	glTranslatef(posicion_x, posicion_y, 0);

}
void animacion(){
	angX+=mod_angX;
	angY+=mod_angY;
	angZ+=mod_angZ;
	
	if(angX>=360)  angX=0;
	if(angY>=360)  angY=0;
	if(angZ>=360)  angZ=0;
	
	glRotatef(angX,1,0,0);
	glRotatef(angY,0,1,0);
	glRotatef(angZ,0,0,1);
	
}
void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glTranslated(0,0,-600);
	fondo();
	glLoadIdentity();
	
	animacion();
	Fig3d(TexCaja,210,450,10,40,70,3.5);
	
	Fig3d(TexNube,400,300,-560,80,140,3);
	Fig3d(TexNube,-180,400,-560,80,140,3);
	
	Letra(TexLetra_Mario,-520,530,-560);
	Letra(TexLetra_Moneda,-380,530,-560);
	
	animacion();
	Fig3d(TexLadrillo,310,100,10,50,100,0);
	animacion();
	Fig3d(TexLadrillo,210,100,10,50,100,0);
	animacion();	
	Fig3d(TexLadrillo,110,100,10,50,100,0);
	animacion();
	Fig3d(TexLadrillo,-380,100,10,50,100,0);
	animacion();
	Fig3d(TexLadrillo,-280,100,10,50,100,0);
	animacion();
	Fig3d(TexLadrillo,-180,100,10,50,100,0);
	
	//ladrillos piso
	animacion();
	Fig3d(TexLadrillo,100,-560,10,50,100,0);
	animacion();
	Fig3d(TexLadrillo,200,-560,10,50,100,0);
	animacion();
	Fig3d(TexLadrillo,300,-560,10,50,100,0);
	animacion();
	Fig3d(TexLadrillo,400,-560,10,50,100,0);
	animacion();
	Fig3d(TexLadrillo,500,-560,10,50,100,0);
	animacion();
	Fig3d(TexLadrillo,600,-560,10,50,100,0);
	animacion();
	Fig3d(TexLadrillo,-100,-560,10,50,100,0);
	animacion();
	Fig3d(TexLadrillo,-200,-560,10,50,100,0);
	animacion();
	Fig3d(TexLadrillo,-300,-560,10,50,100,0);
	animacion();
	Fig3d(TexLadrillo,-400,-560,10,50,100,0);
	animacion();
	Fig3d(TexLadrillo,-500,-560,10,50,100,0);
	animacion();
	Fig3d(TexLadrillo,-600,-560,10,50,100,0);
	//tubo
	animacion();
	Fig3d(TexTubo,420,-420,10,40,120,0);
	animacion();
	Fig2d(TexEstrella,220,230,10,30,70,3.5);
	animacion();
	Fig2d(TexMoneda,-380,240,10,30,70,3.5);
	animacion();
	Fig2d(TexMoneda,-280,240,10,30,70,3.5);
	animacion();
	Fig2d(TexMoneda,-180,240,10,30,70,3.5);

	animacion();
	yoshi(TexYoshi,50,150,3.5);
	
	animacion();
	Fig2d(TexFantasma,-180,-400,10,50,150,3.5);
	
	animacion();
	Hongo();
	
	animacion();
	glTranslatef(110,220,35);
	ojo(4,2.5);
	glLoadIdentity();
	animacion();
	glTranslatef(130,220,35);
	ojo(4,2.5);

	glLoadIdentity();
	animacion();
	glTranslatef(420,-300,10);
	glRotatef(180,0,0,1);
	Esfera(TexPlantaCarnivora,30);
	
	glLoadIdentity();
	animacion(); 
	animacionPelota();
	glScalef(1.0,1.5,1.0);
	glRotatef(90,1,0,0);
	glRotatef(180, 0, 1, 0);
	glRotatef(angulo*4,0,1,0);	
    Esfera(TexCaraEsfera,50);
	glLoadIdentity();	
	glutSwapBuffers();
}

void reshape(int largo, int ancho){
  glViewport(0,0,(GLsizei)largo,(GLsizei)ancho);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  int t=700;
  glOrtho(-t,t,-t,t,-t,t);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y){
	switch(key){
		case 'w':
			mod_angX -= 0.00002;
			break;
		case 's':
			mod_angX += 0.00002;
			break;
		case 'a':
			mod_angY -= 0.00002;
			break;
		case 'd':
			mod_angY += 0.00002;
			break;
		case ' ':
			mod_angX=0;
			mod_angY=0;
			mod_angZ=0;
			break;
	}
}

void keyboard_s(int key, int x, int y){
	switch(key){
		case GLUT_KEY_UP:
			mod_angYPelota+=0.005;
			break;
		case GLUT_KEY_DOWN:
			mod_angYPelota-=0.005;
			break;
		case GLUT_KEY_LEFT:
			mod_angXPelota-=0.005;
			break;
		case GLUT_KEY_RIGHT:
			mod_angXPelota+=0.005;
			break;	
		case GLUT_KEY_END:
			mod_angXPelota=0;
			mod_angYPelota=0;
			break;
	}
}

void idle(){
	angulo+=0.02;
	display();
}

int main(int argc, char** argv) {
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA |GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600,600);
	glutInitWindowPosition(500,100);
	glutCreateWindow("Examenn-Finalechi");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard_s);
	glutIdleFunc(idle);
	sndPlaySound("Musica_Mario_Bros.wav",SND_ASYNC);
	glutMainLoop();
	return 0;
}
