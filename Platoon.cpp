#include "Platoon.h"
#include <iostream>

Platoon::Platoon(string init)
{
	// the string 'init' looks like:
	// a,b ; c,d ; ... where, a and c are the vehicle id, b and d are positions.
	stringstream ss(init);
	string segment;
	while(getline(ss, segment, ';'))
	{
		size_t found = segment.find(",");
		if (found!=std::string::npos){
			string id = segment.substr(0, found);
			string pos = segment.substr(found+1, segment.size()-1);

			// conversion to integer
			stringstream s1(id);
			int id_value;
			s1 >> id_value;
			stringstream s2(pos);
			int pos_value;
			s2 >> pos_value;
			Car* car = new Car(id_value, pos_value);

			insert(car);
		}
	}
}

Car* Platoon::get_tail()
{
	return tail;
}

Car* Platoon::get_head()
{
	return head;
}

void Platoon::set_tail(Car *car)
{
	tail = car;
}

void Platoon::set_head(Car *car)
{
	head = car;
}

/*
 * Removes a car object from the linked list of its platoon.
 * Also supports cases removing a head, tail, or both.
 */
void Platoon::remove(Car* car) {

	// remove last member of list
	if ((car == head) && (car == tail)) {
		tail = NULL;
		head = NULL;
	}

	// remove head
	else if (car == head) {
		head = car->get_next();
		head->set_prev(NULL);
	}

	// remove tail
	else if (car == tail) {
		tail = car->get_prev();
		tail->set_next(NULL);
	}

	// remove non-edge case
	else {
		Car *nextCar = car->get_next();
		Car *prevCar = car->get_prev();

		prevCar->set_next(nextCar);
		nextCar->set_prev(prevCar);
	}

}

/*
 * Inserts a car into a linked list, by using its position.
 * Supports cases where position causes appending/prepending, including empty lists.
 */
void Platoon::insert(Car *car){

	// empty list case
	if (head == NULL) {
		head = car;
		tail = car;
		car->set_prev(NULL);
		car->set_next(NULL);
		return;
	}

	// insert new car behind existing car in list with higher position, if one exists
	for (Car *nextCar = head ; nextCar != NULL ; nextCar = nextCar->get_next()) {

		if (nextCar->get_position() > car->get_position()){
			Car *prevCar = nextCar->get_prev();

			car->set_next(nextCar);
			car->set_prev(prevCar);
			nextCar->set_prev(car);

			if (prevCar == NULL){
				head = car;
			} else {
				prevCar->set_next(car);
			}

			return;
		}
	}

	// if there is no car with higher position, insert at tail of list
	car->set_prev(tail);
	tail->set_next(car);
	tail = car;
	car->set_next(NULL);
}

/*
 * Checks if any of the cars in the platoon occupy a given position
 */
bool Platoon::isEmpty(int pos){

	if (this->head == NULL){
		return true;
	}

	// Loop through each car and compare position values
	for (Car *thisCar = this->head ; thisCar != NULL ; thisCar = thisCar->get_next()){

		if (thisCar->get_position() == pos){
			return false;
		}
	}

	return true;
}

Platoon::~Platoon(){
	for (Car *thisCar = this->head ; thisCar != NULL ; thisCar = thisCar->get_next()){
		delete thisCar;
	}
}
