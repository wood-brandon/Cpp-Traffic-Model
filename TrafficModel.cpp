#include "TrafficModel.h"
#include <iostream>
#define LEFT_SIGNAL 1
#define RIGHT_SIGNAL 2

TrafficModel::TrafficModel() { }

void TrafficModel::set_commands(vector<string> commands)
{
	this->commands = commands;
}

/* A helper function. 
 * Given a specific id as the input, it searches the lane change command stored in
 * "commands". Returns the value of the turn-light in the command for this car id.
 * 0 = no light, 1 = left light, 2 = right light.
 */
int TrafficModel::get_lane_change_command(int id)
{
	int count = commands.size();
	for (int i = 0; i < count; i++){
		size_t found = commands[i].find(',');
		string iter_id = commands[i].substr(0, found);
		string light = commands[i].substr(found+1, commands[i].size()-1);
		stringstream ss1(iter_id);
		int id_value;
		ss1 >> id_value;
		if (id_value == id){
			stringstream ss2(light);
			int light_value;
			ss2 >> light_value;
			return light_value;
		}
	}
	return 0;
}

/*
 * This function updates all vehicles for the current tick.
 * The update order is top lanes to bottom lanes, and inside lanes rightmost cars to leftmost cars.
 */
void TrafficModel::update()
{

	int size = platoons.size();
	Car *temp = NULL;

	// Iterate through lanes from top to bottom
	for (int lane = 0; lane < size; lane++) {
		// Execute moves in lane from tail to head
		for (Car *thisCar = platoons[lane]->get_tail() ; thisCar != NULL ; thisCar = temp ) {
			// Get position of previous car before moving position in case this changes
			temp = thisCar->get_prev();
			int direction = get_lane_change_command(thisCar->get_id());

			// Set a flag for cars that moved one lane down to prevent moving them again next iteration
			if (thisCar->get_moved()) {
				thisCar->set_moved(false);

			} else {
				doMove(thisCar,direction,lane);

			}




		}


	}

}

/*
 * This function is responsible for moving cars based on a given turn signal.
 * Cars that are not turning or that are unable to turn will move forward (if unblocked).
 */
void TrafficModel::doMove(Car *car,int signal,int lane){

	int maxLane = (platoons.size() - 1);
	switch(signal) {

	case LEFT_SIGNAL :
		// prevent changing into an invalid lane
		if (lane > 0){

			if (changeLane(car,platoons[lane],platoons[lane-1])){
				return;
			}
		}
		break;

	case RIGHT_SIGNAL :

		if (lane < (maxLane)){

			if (changeLane(car,platoons[lane],platoons[lane+1])){
				car->set_moved(true);
				return;
			}
		}
		break;
	}

	// if not signaling or unable to turn, drive forward
	driveForward(car,platoons[lane]);

}

/*
 * This function attempts to change lane, returning TRUE on a success, otherwise false
 * if the lane is occupied (no movement is made in this case).
 */
bool TrafficModel::changeLane(Car *car,Platoon *origin,Platoon *destination){

	int position = car->get_position();

	if (destination->isEmpty(position) == false){
		return false;
	}

	origin->remove(car);

	destination->insert(car);

	return true;
}

/*
 * This function increases the position of the given car, unless this space is occupied by another car.
 */
void TrafficModel::driveForward(Car *car,Platoon *lane){
	int destination = car->get_position() + 1;

	if (lane->isEmpty(destination) == true) {
		car->set_position(destination);
	}

}


/*
 * Initialization based on the input information
 */
void TrafficModel::initialize(vector<string> info)
{
	int lane_count = info.size();
	for (int i = 0; i < lane_count; i++){
		Platoon *p = new Platoon(info[i]);
		platoons.push_back(p);
	}
}

// Returns all the vehicle states in the system
vector<string> TrafficModel::get_system_state()
{
	vector<string> output;
	int size = platoons.size();
	for (int i = 0; i < size; i++){
		// get the last vehicle in the platoon
		Car* temp = platoons[i]->get_head();
		string s = "";
		ostringstream out;
		while (temp != NULL){
			out << ";(" << temp->get_id() << "," << i << "," << temp->get_position() << \
					"," << get_lane_change_command(temp->get_id()) << ")";
			temp = temp->get_next();
		}

		output.push_back(out.str());
	}
	return output;
}

TrafficModel::~TrafficModel(){
	int size = platoons.size();
	for (int i = 0 ; i < size ; i++) {
		delete platoons[i];
	}
}
