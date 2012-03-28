#include "Scene.hpp"
#include "const.hpp"
#include <cstdlib>
#include "ContactListener.hpp"
#include <iostream>
#include "EntityManager.hpp"

Scene::Scene()
{
	physics = new b2World(b2Vec2(0, 0));
	physics->SetContactListener(new ContactListener());
	alg = new Alg(physics);
	running = true;
}

bool Scene::isRunning()
{
	return running;
}

Scene::~Scene()
{
	delete alg;
	delete physics;
}

void Scene::render(sf::RenderTarget *target)
{
	// rendering
	alg->render(target);

	std::vector<Particle*>::iterator it;
	for(it=particles.begin(); it!=particles.end(); it++)
	{
		Particle *p = *it;
		p->render(target);

	}

	// physics step :
	float32 tstep = 1.0f/20.0f;
	if(alg->isAlive())
		physics->Step(tstep,20,20);
	else
		running = false;

	// delete old particles :
	if(particles.size() > 200)
	{
		for(unsigned int i=0; i<100; i++)
		{
			Particle *p = particles.front();
			EntityManager::getInstance()->enqueueToDelete(p);
			particles.erase(particles.begin());
		}
	}
}

void Scene::throwParticle(int nb)
{
	while(nb>0)
	{
		double x = W_WIDTH/2/SCALE;
		double y = rand()%W_HEIGHT;
		y -= W_HEIGHT/2;
		y /= SCALE;

		particles.push_back( new Particle(physics, x, y) );
		nb--;
	}
}