#include <cmath>
#include <cstdlib>
/*#ifdef _WIN32
#include <Windows.h>
#endif*/
#include <gl\gl.h>
#include <gl\glut.h>
#include <GL\glu.h>

#include "tga.h"
#include "solarsystem.h"
#include "camera.h"
#include "globals.h"

// delgetsiin hemjees
int screenWidth, screenHeight;

// TGA classiin objectuud
TGA *help, *stars, *moon;
// Zaavriin zurgiin haragdah esehiig hadgalah boolean huvisagch
bool helpDialogue = false;
// toggles if orbits are drawn
bool showOrbits = true;
// 1-ees 9 hurtel dugaartai gariguudiig songoson esehiig hadgalah huvisagch
int planetSelected = 1;

// Solarsystem classiing object
SolarSystem solarSystem;

// Camera classiin object
Camera camera;

// hugatsaanii simulation-nii huvisagchid
double time;
double timeSpeed;

// delgetsiin camera hodloh esehiig shiideh boolean utguud
struct ControlStates {
	bool forward, backward, left, right, yawLeft, yawRight, pitchUp, pitchDown, rollLeft, rollRight;
} controls;

// 10ms bolgon timer function duudagdana
void timer(int) {
	glutPostRedisplay(); // display() function-iig dahij duudah function
	glutTimerFunc(10, timer, 0); // frame zurahiig 100fps-eer hyzgaarlaj baigaa
}

// max too hurtel random too gargah function
float random(float max) {
	return (float)(std::rand() % 1000) * max * 0.001;
}

// Songogdson garigt sar nemej ogoh function
void addMoon() {
	// random utguudiig ashiglaj sariig uusgej baigaa
	solarSystem.addMoon(planetSelected,
		(500 + random(1500)) * solarSystem.getRadiusOfPlanet(planetSelected),
		10 + random(100), 0.5 + random(20),
		solarSystem.getRadiusOfPlanet(planetSelected) * (0.05f + random(0.2f)), moon->getTextureHandle());
}

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);

	// Gerliin tootsoo
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	GLfloat matSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat matAmbience[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat matShininess[] = { 20.0 };
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbience);

	GLfloat lightAmbient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat lightDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	// Zurgiin data-g unshih code (TGA classiin objectuudiig garig bolgon deer uusgeh)
	help = new TGA("C:/Users/User/OneDrive - National University of Mongolia/Desktop/MUIS/3-r kurs/1) Namriin uliral/Computer graphics/laboratories/SolarSystem/images/help.tga");
	stars = new TGA("C:/Users/User/OneDrive - National University of Mongolia/Desktop/MUIS/3-r kurs/1) Namriin uliral/Computer graphics/laboratories/SolarSystem/images/stars6.tga");
	moon = new TGA("C:/Users/User/OneDrive - National University of Mongolia/Desktop/MUIS/3-r kurs/1) Namriin uliral/Computer graphics/laboratories/SolarSystem/images/moon.tga");

	TGA* earth = new TGA("C:/Users/User/OneDrive - National University of Mongolia/Desktop/MUIS/3-r kurs/1) Namriin uliral/Computer graphics/laboratories/SolarSystem/images/earth.tga");
	TGA* sun = new TGA("C:/Users/User/OneDrive - National University of Mongolia/Desktop/MUIS/3-r kurs/1) Namriin uliral/Computer graphics/laboratories/SolarSystem/images/sun.tga");
	TGA* mercury = new TGA("C:/Users/User/OneDrive - National University of Mongolia/Desktop/MUIS/3-r kurs/1) Namriin uliral/Computer graphics/laboratories/SolarSystem/images/mercury.tga");
	TGA* venus = new TGA("C:/Users/User/OneDrive - National University of Mongolia/Desktop/MUIS/3-r kurs/1) Namriin uliral/Computer graphics/laboratories/SolarSystem/images/venus.tga");
	TGA* mars = new TGA("C:/Users/User/OneDrive - National University of Mongolia/Desktop/MUIS/3-r kurs/1) Namriin uliral/Computer graphics/laboratories/SolarSystem/images/mars.tga");
	TGA* jupiter = new TGA("C:/Users/User/OneDrive - National University of Mongolia/Desktop/MUIS/3-r kurs/1) Namriin uliral/Computer graphics/laboratories/SolarSystem/images/jupiter.tga");
	TGA* saturn = new TGA("C:/Users/User/OneDrive - National University of Mongolia/Desktop/MUIS/3-r kurs/1) Namriin uliral/Computer graphics/laboratories/SolarSystem/images/saturn.tga");
	TGA* uranus = new TGA("C:/Users/User/OneDrive - National University of Mongolia/Desktop/MUIS/3-r kurs/1) Namriin uliral/Computer graphics/laboratories/SolarSystem/images/uranus.tga");
	TGA* neptune = new TGA("C:/Users/User/OneDrive - National University of Mongolia/Desktop/MUIS/3-r kurs/1) Namriin uliral/Computer graphics/laboratories/SolarSystem/images/neptune.tga");
	TGA* pluto = new TGA("C:/Users/User/OneDrive - National University of Mongolia/Desktop/MUIS/3-r kurs/1) Namriin uliral/Computer graphics/laboratories/SolarSystem/images/pluto.tga");

	// Gariguudiig SolarSystem-d nemej baigaa, Zaig km-eer, Hugatsaag odroor geh met.
	solarSystem.addPlanet(0, 1, 500, 695500, sun->getTextureHandle()); // sun
	solarSystem.addPlanet(57910000, 88, 58.6, 2440, mercury->getTextureHandle()); // mercury
	solarSystem.addPlanet(108200000, 224.65, 243, 6052, venus->getTextureHandle()); // venus
	solarSystem.addPlanet(149600000, 365, 1, 6371, earth->getTextureHandle()); // earth
	solarSystem.addPlanet(227939100, 686, 1.03f, 3389, mars->getTextureHandle()); // mars
	solarSystem.addPlanet(778500000, 4332, 0.4139, 69911, jupiter->getTextureHandle()); // jupiter
	solarSystem.addPlanet(1433000000, 10759, 0.44375, 58232, saturn->getTextureHandle()); // saturn
	solarSystem.addPlanet(2877000000, 30685, 0.718056, 25362, uranus->getTextureHandle()); // uranus
	solarSystem.addPlanet(4503000000, 60188, 0.6713, 24622, neptune->getTextureHandle()); // neptune
	solarSystem.addPlanet(5906380000, 90616, 6.39, 1137, pluto->getTextureHandle()); // pluto

	solarSystem.addMoon(3, 7000000, 27.3, 27.3, 1738, moon->getTextureHandle()); // test moon for the earth

	// Hugatsaag tohiruulah
	time = 2.552f;
	timeSpeed = 0.1f;

	// Controlleruudiig reset hiih
	controls.forward = false;
	controls.backward = false;
	controls.left = false;
	controls.right = false;
	controls.rollRight = false;
	controls.rollLeft = false;
	controls.pitchDown = false;
	controls.pitchUp = false;
	controls.yawLeft = false;
	controls.yawRight = false;

	timer(0);
}

// Orchinoo beldeh function
void drawGalaxy(void);

void display(void) {
	// Simulation-iig zurah bolgondoo update hiih
	time += timeSpeed;
	solarSystem.calculatePositions(time);

	if(controls.forward) camera.forward();		if(controls.backward) camera.backward();
	if(controls.left) camera.left();			if(controls.right) camera.right();
	if(controls.yawLeft) camera.yawLeft();		if(controls.yawRight) camera.yawRight();
	if(controls.rollLeft) camera.rollLeft();	if(controls.rollRight) camera.rollRight();
	if(controls.pitchUp) camera.pitchUp();		if(controls.pitchDown) camera.pitchDown();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	// 3d orchinoo render hiihed zoriulj perspective matrixaa tohiruulah
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0f, (float)screenWidth/(float)screenHeight, 0.001f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Camera-iin orientation huvirgaltiig guitsetgeh
	camera.transformOrientation();

	// Galaxy orchinoo beldeeh
	glBindTexture(GL_TEXTURE_2D, stars->getTextureHandle());
	drawGalaxy();

	// Camera-iin translation huvirgaltiig guitsetgeh
	camera.transformTranslation();

	GLfloat lightPosition[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	// Narnii system-iig renderleh
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	solarSystem.render();
	glDisable(GL_LIGHTING);

	// garig bolgonii orbit-iig renderleh
	if(showOrbits)
		solarSystem.renderOrbits();

	glDisable(GL_DEPTH_TEST);

	// User Interface-iig haruulah ortho matritsiig tohiruulah (Zaavriin zurag)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble) screenWidth, (GLdouble) screenHeight, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Zaavriin zurgiig zurah
	if (helpDialogue) {
		glBindTexture(GL_TEXTURE_2D, help->getTextureHandle());
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);	glVertex2f(0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);	glVertex2f(512.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);	glVertex2f(512.0f, 512.0f);
			glTexCoord2f(0.0f, 1.0f);	glVertex2f(0.0f, 512.0f);
		glEnd();
	}

	controls.yawLeft = false;
	controls.yawRight = false;
	controls.pitchDown = false;
	controls.pitchUp = false;

	glFlush();
	glutSwapBuffers();
}
void keyDown(unsigned char key, int x, int y) {
	if (key > '0' && key <= '9') {
		float vec[3];
		solarSystem.getPlanetPosition(key - '0', vec);
		camera.pointAt(vec);

		planetSelected = key - '0';
	}
	switch (key) {
        case '-':
            timeSpeed /= 2.0f; // Tsag hugatsaag 2 dahin bagasgah
            break;
        case '=':
            timeSpeed *= 2.0f; // Tsag hugatsaag 2 dahin hurdluulah
            break;
        case 'h':
            helpDialogue = !helpDialogue; // Zaavriin zurgiig toggle hiih
            break;
        case '[':
            planetSizeScale /= 1.2; // Garigsiig jijigruuleh
            break;
        case ']':
            planetSizeScale *= 1.2; // Garigsiig tomruulah
            break;
        case 'o':
            showOrbits = !showOrbits; // Garigsiin orbit-iig toggle hiih
            break;
        case 'm':
            addMoon(); // songogdson garigt sar nemeh
            break;
        case 'r':
            planetSizeScale = distanceScale;
            break;
        case ',':
            camera.slowDown(); // camera-nii hurdiig 2 dahin bagasgah
            break;
        case '.':
            camera.speedUp(); // camera-nii hurdiig 2 dahin hurdasgah
            break;
        case 'w':
            controls.forward = true;
            break;
        case 's':
            controls.backward = true;
            break;
        case 'a':
            controls.left = true;
            break;
        case 'd':
            controls.right = true;
            break;
        case 'l':
            controls.yawRight = true;
            //controls.rollRight = true;
            break;
        case 'j':
            controls.yawLeft = true;
            //controls.rollLeft = true;
            break;
        case 'i':
            controls.pitchDown = true;
            break;
        case 'k':
            controls.pitchUp = true;
            break;
        /*case 'q':
            controls.yawLeft = true;
            break;
        case 'e':
            controls.yawRight = true;
            break;*/
        }
}

void keyUp(unsigned char key, int x, int y) {
	switch (key) {
        case 'w':
            controls.forward = false;
            break;
        case 's':
            controls.backward = false;
            break;
        case 'a':
            controls.left = false;
            break;
        case 'd':
            controls.right = false;
            break;
        case 'l':
            controls.yawRight = false;
            //controls.rollRight = false;
            break;
        case 'j':
            controls.yawLeft = false;
            //controls.rollLeft = false;
            break;
        case 'i':
            controls.pitchDown = false;
            break;
        case 'k':
            controls.pitchUp = false;
            break;
        /*case 'q':
            controls.yawLeft = false;
            break;
        case 'e':
            controls.yawRight = false;
            break;*/
	}
}

void reshape(int w, int h) {
	screenWidth = w;
	screenHeight = h;
	glViewport (0, 0, (GLsizei)w, (GLsizei)h);
}

int mouseX = 600, mouseY = 350;
double yLimit = 350;

void Mouse(int x, int y) {
    if(x < mouseX) {
        controls.yawLeft = true;
    }
    if(x > mouseX) {
        controls.yawRight = true;
    }
    if(y > mouseY) {
        if(yLimit > 0) {
            controls.pitchDown = true;
            yLimit -= 0.7;
        }
    }
    if(y < mouseY) {
        if(yLimit < 700) {
            controls.pitchUp = true;
            yLimit += 0.7;
        }
    }
    glutWarpPointer(1200 / 2, 700 / 2);
    //mouseX = x;
    //mouseY = y;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1200, 700);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutSetCursor(GLUT_CURSOR_NONE);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutPassiveMotionFunc(Mouse);
	//glutMotionFunc(Mouse);
	glutMainLoop();
	return 0;
}

void drawGalaxy(void) {
    GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricTexture(quadric, true);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluSphere(quadric, 1, 30, 30);
	/*glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);	glVertex3f(-1.0f, -1.0f, 1.0f);
        glTexCoord2f(1.0f, 0.0f);	glVertex3f(1.0f, -1.0f, 1.0f);
        glTexCoord2f(1.0f, 1.0f);	glVertex3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f);	glVertex3f(-1.0f, 1.0f, 1.0f);

        glTexCoord2f(0.0f, 0.0f);	glVertex3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(1.0f, 0.0f);	glVertex3f(1.0f, 1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f);	glVertex3f(1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f);	glVertex3f(1.0f, -1.0f, 1.0f);

        glTexCoord2f(0.0f, 0.0f);	glVertex3f(1.0f, 1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f);	glVertex3f(-1.0f, 1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f);	glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f);	glVertex3f(1.0f, -1.0f, -1.0f);

        glTexCoord2f(0.0f, 0.0f);	glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f);	glVertex3f(-1.0f, -1.0f, 1.0f);
        glTexCoord2f(1.0f, 1.0f);	glVertex3f(-1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f);	glVertex3f(-1.0f, 1.0f, -1.0f);

        glTexCoord2f(0.0f, 0.0f);	glVertex3f(-1.0f, 1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f);	glVertex3f(1.0f, 1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f);	glVertex3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f);	glVertex3f(-1.0f, 1.0f, 1.0f);

        glTexCoord2f(0.0f, 0.0f);	glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f);	glVertex3f(1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f);	glVertex3f(1.0f, -1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f);	glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();*/
}
