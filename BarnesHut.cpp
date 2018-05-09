
#include "BarnesHut.hpp"

BarnesHut::BarnesHut(int N) {
    InitiateBodies(N);
}

/**
 *
 */
BarnesHut::~BarnesHut() {

}

void BarnesHut::InitiateBodies(int N) {
//    bodies.resize(N); //create the arrays of bodies

 m_bodies.push_back(Body(0, 0, 0, 0, solarmass)); // put a sun in the middle
    m_bodies.push_back(Body(1.4959826e8, 0, 0, 29.783, 5.9722e24)); //earth
    m_bodies.push_back(Body(1.4959826e8, -384400, 1.022, 29.783, 7.3477e22)); //moon

    for (int i = 0; i < N; i++) {
        double px = radius * exp(-1.8) * (.5 - ((double) rand() / (RAND_MAX)))*2;
        double py = radius * exp(-1.8) * (.5 - ((double) rand() / (RAND_MAX)))*2;

        double magv = circlev(px, py);

        double absangle = atan(abs(py / px));
        double thetav = M_PI / 2 - absangle;
//        double phiv = ((double) rand() / (RAND_MAX)) * M_PI;
        // double vx = -1 * Math.signum(py) * Math.cos(thetav) * magv;
        // double vy = Math.signum(px) * Math.sin(thetav) * magv;
        double vx = -1 * cos(thetav) * magv;
        double vy = sin(thetav) * magv;
        // Orient a random 2D circular orbit

        // if (Math.random() <=.5) {
        // vx=-vx;
        // vy=-vy;
        // }
        double mass = ((double) rand() / (RAND_MAX)) * solarmass * 1e-4 + 7e20;
        //double mass = 5.9722e24;

        m_bodies.push_back(Body(px, py, vx, vy, mass));

    }



}

double BarnesHut::circlev(double x, double y) {
    double r2 = sqrt(x * x + y * y)*1000;
    double numerator = GlobalConst::G * GlobalConst::SolarMass ;
    return sqrt(numerator / r2 ) / 1000;
    // return Math.sqrt(numerator / r2) / 1000;
}

// The BH algorithm: calculate the forces
	void BarnesHut::addForce() {
		// thetree = new BHTree(q);
		// // If the body is still on the screen, add it to the tree
		// for (int i = 0; i < N; i++) {
		// if (bodies[i].in(q))
		// thetree.insert(bodies[i]);
		// bodies[i].resetForce();
		//
		// }
		// Now, use out methods in BHTree to update the forces,
		// traveling recursively through the tree
		// for (int i = 0; i < N; i++) {
		// if (bodies[i].in(q)) {
		// thetree.updateForce(bodies[i]);
		//
		// }
		//
		// }
		// for (int i = 0; i < N; i++) {
		// // Calculate the new positions on a time step dt (1e11 here)
		// // bodies[i].update(1e11);
		// bodies[i].update(86400/20); // update a day
		// }
		for (int k = 0; k < TIMESTEP_X; k++) {
			for (unsigned int i = 0; i < m_bodies.size(); i++) {

				m_bodies[i].resetForce();
				// Notice-2 loops-->N^2 complexity
				for (unsigned int j = 0; j < m_bodies.size(); j++) {
					if (i != j)


						m_bodies[i].addAttractionOf(m_bodies[j]);
				}
			}
			// Then, loop again and update the bodies using timestep dt
			for (unsigned int i = 0; i < m_bodies.size(); i++) {

				// if (t == 0) {
				// //init the half-way vector
				// bodies[i].initVector(TIMESTEP);
				// }
//				if (m_bodies[i].exist) {
					m_bodies[i].update(TIMESTEP);
//				}
			}
//			t++;
		}

		// if (t%365 == 0) {
		// //output all 365 day
		// System.out.println("a year hav passed, earth position y position :"+bodies[1].ry);
		// }
	}
        std::vector<Body>* BarnesHut::getBodies() {
            return &m_bodies;
        }
