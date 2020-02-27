# JuniorDesign
**Swarm Bot Project**

The goal of this semester-long project is to model and simulate the navigation and environmental sensing of autonomous vehicles by designing an intelligent swarmbot
team using a community of two bots. The two bots are to work together, collaborating to solve
the problem of safe navigation.

## Libraries:

**FourPinControl** - setup four pins as OUTPUT then control them using public functions. KINDA WORKS.

**PinControl** - setup one pin as OUTPUT and control it using public functions. WORKS but do we need this?

**GroundSensor** - handles color sensing and outputs a command.
<pre><code>Inputs: 
  - Bounds for blue and red LEDs voltage reading
    - NOTE: will need to change these a lot based on testing and observation
  - Instructions on what the command should be for each color sense
  NOTE: Now handles color transition
</code></pre>

**MotorControl** - controls the motors.
<pre><code>Inputs: 
  - Wheels' pins. Indicate four pins for left and right motors
  - Speeds for each direction of movement
  - Accel holds the accelartion time and acceleration speed
  - Adjust helps make the wheels travel straighter
  NOTE: Now handles acceleartion and individual wheel's speed adjustments
</code></pre>

## Relevant Folders:

**AnalogInReadTest** - takes in voltage reading and print out their their Arduino value on Serial monitor.

**DistanceTest** - makes the bot move forward or backward for a certain time.

**DriveMotorTest** - drive motors forward and backward at certain speed from Serial input.

**FollowLineUsingLib** - make the bot do certain tasks based on the color it sensed. Use custom libraries.

**GroundSensorTest** - use the color sensor to sense color of tapes. Print result on Serial monitor.

**HallSensorTest** - use the hall sensor to stop when it sense a magnetic field. UNTESTED!

**HallSensorTestBasic** - light up an LED based on hall sensor reading. UNTESTED!

**MotionControl** - control bots movement using Serial input. 

**SearchingTest** - test some searching algorithm.

## Old Folders:

~~DriveMotorMiniTest~~ - turn an LED on and off using Serial input.

~~FollowTheLine~~ - make bot do certain tasks based on color it sensed. OUTDATED!

~~GenerateOutOfPhaseTest~~ - tried to generate two signals out of phase. OUTDATED! UNECCESSARY!

~~LockedAntiphase~~ - drive two motors. OUTDATED!

~~StateMachine~~ - light up some LED based on the state. State is change using Serial input. OLD!
