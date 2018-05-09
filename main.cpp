/* 
 * File:   main.cpp
 * Author: eephyne
 *
 * Created on 6 juillet 2013, 18:11
 */

#include <iostream>
#include <GL/gl.h>
#include <SDL/SDL.h>
#include <GL/glu.h>
#include <ctime>
#include <deque>
#include "inc/Component.h"
#include "inc/ComponentManager.h"
#include "inc/Entity.h"
#include "inc/EntityKey.h"
#include "inc/BarnesHut.hpp"
#include "inc/MovementSystem.h"
#include "inc/ForceSystem.h"
#include "inc/MathObjects.h"
#include "inc/GlobalConst.h"
#include <cmath>
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
	int FPS = 30;
	Uint32 start_time;
	SDL_Init(SDL_INIT_VIDEO);
//antialiasing
SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
glEnable(GL_MULTISAMPLE);

	SDL_Surface* ecran = SDL_SetVideoMode(600, 600, 32, SDL_OPENGL);

	SDL_Flip(ecran);

	SDL_Event event;

	BarnesHut bh = BarnesHut(50);

	//start of testing entity system

	std::vector<Entity*> m_entities;
	EntityKey EKPosition = EntityKey::set(GlobalConst::CURSOR_POSITION,1);
	EntityKey EKSwitch = EntityKey::set(GlobalConst::CURSOR_SWITCH,0);
	//m_entities.reserve(10);
	for (int i = 0; i < 100; ++i)
	{
		double px = 4.503e9 * exp(-1.8) * (.5 - ((double) rand() / (RAND_MAX)))*2;
		double py = 4.503e9 * exp(-1.8) * (.5 - ((double) rand() / (RAND_MAX)))*2;

		double magv = BarnesHut::circlev(px,py);
		double absangle = atan2(py,px);
		double thetav = M_PI / 2 - absangle;
		double vx = -1 * cos(thetav) * magv;
		double vy = sin(thetav) * magv;
		// Orient a random 2D circular orbit
		

		// if (Math.random() <=.5) {
		// vx=-vx;
		// vy=-vy;
		// }
		double mass = ((double) rand() / (RAND_MAX)) * 1.98892e30 * 1e-4 + 7e20;
		Entity *entity = new Entity();
		ekey *key = entity->getKey();
		EKPosition.apply(*key);
		EKSwitch.apply(*key);
		ComponentManager *cm = entity->getComponentManager();

		vector2d *pos = new vector2d();
		pos->x = px;
		pos->y = py;
		cm->set<vector2d*>("position",pos);

		vector2d *vel = new vector2d() ;
		vel->x = vx;
		vel->y = vy;
		cm->set<vector2d*>("velocity",vel);

		vector2d *force = new vector2d();
		force->x = 0;
		force->y = 0;
		cm->set<vector2d*>("force",force);

		//		double mass = 0;
		cm->set<double>("mass",mass);

		std::deque<vector2d> *trace = new std::deque<vector2d>();
		cm->set<std::deque<vector2d>* >("trace",trace);
	//	m_entities[i] = entity;
	m_entities.push_back(entity);
	}
	//place the sun
	Entity *sun = m_entities[0];
sun->getComponentManager()->get<vector2d*>("position")->x = 0;	
sun->getComponentManager()->get<vector2d*>("position")->y = 0;	
sun->getComponentManager()->get<vector2d*>("velocity")->x = 0;	
sun->getComponentManager()->get<vector2d*>("velocity")->y = 0;	
sun->getComponentManager()->set<double>("mass",GlobalConst::SolarMass);	
sun = m_entities[1];
sun->getComponentManager()->set<double>("mass",GlobalConst::SolarMass/10); //test : double sun	
sun->getComponentManager()->get<vector2d*>("velocity")->x = 0;	
sun->getComponentManager()->get<vector2d*>("velocity")->y = 0;	
	//todo systems
	MovementSystem ms = MovementSystem(&m_entities);
	ForceSystem fs = ForceSystem(&m_entities);
	//end of testing entity system

	for (;;) {
		start_time = SDL_GetTicks();

		while (SDL_PollEvent(&event)) {

			switch (event.type) {
				case SDL_QUIT:
					exit(0);
					break;
			}
		}
		//do drawing stuff
		//first animate once the system
		//bh.addForce(); //CHANGE FUNCTION NAME
		//        

		GLUquadric* params;
		params = gluNewQuadric();

		//std::vector<Body> *bodies = bh.getBodies();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode( GL_MODELVIEW );
		double scale = 1/bh.radius*4; //get the scale for adapting position ,
		//for (unsigned int i = 0; i < bodies->size(); i++) {
		//	double size = 0.01;
		//	glLoadIdentity( );
		//	glColor3ub(255, 255, 255);
		//	//trace the trace

		//	glBegin(GL_LINE_STRIP);
		//	for (unsigned int j = 0 ;j < (*bodies)[i].getTrace().size();j+=3) {

		//		glVertex3d((*bodies)[i].getTrace()[j],(*bodies)[i].getTrace()[j+1],(*bodies)[i].getTrace()[j+2]);
		//		//                    glVertex3d((*bodies)[i].getTrace()[j],(*bodies)[i].getTrace()[j+1],(*bodies)[i].getTrace()[j+2]);
		//		//                    cout << (*bodies)[i].getTrace()[j] << " "<< (*bodies)[i].getTrace()[j+1] << " "<< (*bodies)[i].getTrace()[j+2] << endl;
		//	}
		//	glEnd();
		//	glTranslated((*bodies)[i].getX()*scale,(*bodies)[i].getY()*scale,0);


		//	switch (i) {
		//		case 0:
		//			//the sun
		//			glColor3ub(255, 0, 0);
		//			size = 0.04;
		//			break;
		//		case 1:
		//			//the earth
		//			glColor3ub(0, 0, 255);
		//			size = 0.02;
		//			break;
		//		case 2:
		//			//the moon
		//			glColor3ub(150, 150, 250);
		//			size = 0.005;
		//			break;
		//		default:
		//			glColor3ub(255, 255, 255);

		//			break;
		//	}
		//	//cout << (*bodies)[i].getX()*scale << " "<<(*bodies)[i].getY()*scale << endl;
		//	gluDisk(params, 0, size, 20, 1);

		//}
// clock_t Start = clock();
		fs.update();
//		std::cout << "force time:" << clock() - Start;
//		  Start = clock();
		ms.update(86400);
//		std::cout << "update time:" << clock() - Start << std::endl;
		for (unsigned int i = 0; i < m_entities.size(); ++i)
		{
			glLoadIdentity();

			glColor3ub(255, 255, 255);
			//trace the trace

		//	glBegin(GL_LINE_STRIP);
		//	std::deque<vector2d> * trace = m_entities.at(i)->getComponentManager()->get<std::deque<vector2d>* >("trace");
		//	for (unsigned int j = 0 ;j < trace->size();j++) {

		//		glVertex2d(trace->at(j).x*scale,trace->at(j).y*scale);
		//	}
		//	glEnd();
			double x,y;
			x = m_entities[i]->getComponentManager()->get<vector2d*>("position")->x;
			y = m_entities[i]->getComponentManager()->get<vector2d*>("position")->y;
			glColor3ub(0, 255, 0); //green
			glTranslated(x*scale,y*scale,0);
			if (i == 0 || i == 1)
			{
				glColor3ub(255,0,0);
				gluDisk(params, 0, 0.04, 20, 1);

			}			else 
			{
				gluDisk(params, 0, 0.01, 20, 1);

			}
		}

		glFlush();
		SDL_GL_SwapBuffers();
		//end of drawing stuff

		//        SDL_WaitEvent(&event);
		int ellapsed_time = SDL_GetTicks() - start_time;
		if (ellapsed_time < 1000 / FPS) {
			SDL_Delay(1000 / FPS - ellapsed_time);
		}
	}

	SDL_Quit();

	return 0;
}

