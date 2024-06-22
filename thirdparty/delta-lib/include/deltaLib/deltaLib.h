#ifndef DELTA_TIME_H
#define DELTA_TIME_H

/**
* @brief DeltaTime class is used to calculate the time between frames to make the movement of objects independent of the frame rate.
 */
class DeltaTime
{
private:
	double prev_time;
	double curr_time;
public:
	DeltaTime();
	double getDelta();
	/**
	* @brief Needs to be called at the beginning of each frame
	* in order to get the duration of the last frame later by calling deltaTime.delta()
	 */
	void update();
};

#endif // DELTA_TIME_H
