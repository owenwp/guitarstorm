/*
 This file is part of Guitar Storm.
 Copyright 2008 Zombie Process, Owen Pedrotti
 
 Guitar Storm is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 Guitar Storm is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with Guitar Storm.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Threads.h"
#include <pthread.h>
#include <iostream>

void* runThread(void* r)
{
	std::cout << "Running thread\n";
	((runnable*)r)->run();
}

thread::thread(void (*run)())
{
	id = new pthread_t;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	std::cout << "Init thread\n";
	pthread_create((pthread_t*)id, NULL, (void* (*)(void*))run, NULL);
	
	pthread_attr_destroy(&attr);
}

thread::thread(runnable* run)
{	
	id = new pthread_t;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	std::cout << "Init thread\n";
	pthread_create((pthread_t*)id, NULL, runThread, run);
	
	pthread_attr_destroy(&attr);
}

thread::~thread()
{
	cancel(); 
	join(); 
	delete (pthread_t*)id;
}

void thread::cancel()
{
	pthread_cancel(*((pthread_t*)id));
}

void thread::join()
{
	pthread_join(*((pthread_t*)id), NULL);
}

mutex::mutex()
{
	id = new pthread_mutex_t;
	pthread_mutex_init((pthread_mutex_t*)id, NULL);
}

mutex::~mutex()
{
	pthread_mutex_destroy((pthread_mutex_t*)id);
	delete (pthread_mutex_t*)id;
}

void mutex::lock()
{
	pthread_mutex_lock((pthread_mutex_t*)id);
}

void mutex::unlock()
{
	pthread_mutex_unlock((pthread_mutex_t*)id);
}
