#include "Car.h"

Car::Car(int id, int pos){
	this->id = id;
	this->position = pos;
	this->moved = false;
}

int Car::get_id()
{
	return id;
}

int Car::get_position()
{
	return position;
}

void Car::set_position(int pos)
{
	this->position = pos;
}

void Car::set_moved(bool flag){
	moved = flag;
}

bool Car::get_moved(){
	return moved;
}

void Car::set_next(Car* v)
{
	next = v;
}

void Car::set_prev(Car* v)
{
	prev = v;
}

Car* Car::get_next()
{
	return next;
}

Car* Car::get_prev()
{
	return prev;
}
