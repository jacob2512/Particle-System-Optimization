#pragma once

#include "Math\Matrix.h"
#include "Math\Vect4D.h"
#include "Particle.h"

#include <list>

class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter();
	
	void SpawnParticle();
	void update();
	void draw();

	void addParticleToList(Particle *p );
	void removeParticleFromList( Particle *p );

	void Execute(Vect4D& pos, Vect4D& vel, Vect4D& sc);

private:
	
	const Vect4D	start_position = Vect4D(0.0, 0.0, 0.0);
	const Vect4D	start_velocity = Vect4D(0.0, 1.0, 0.0);
	
	const double	max_life;
	const int		max_particles;
	const double	spawn_frequency;

	double	last_spawn;
	double	last_loop;
	int		last_active_particle;

	// added for speed efficiency
	int bufferCount;
  Vect4D	vel_variance;
  Vect4D	pos_variance;
	const double	scale_variance = 2.5;
	std::list<Particle> drawBuffer;
	Particle *headParticle;
	const std::list<Particle> particle_list;

};
