import com.leapmotion.leap.CircleGesture;
import com.leapmotion.leap.Gesture.State;
import com.leapmotion.leap.Gesture.Type;
import com.leapmotion.leap.Hand;
import com.leapmotion.leap.SwipeGesture;
import com.onformative.leap.LeapMotionP5;
import processing.serial.*; 

Serial port;
LeapMotionP5 leap;

String lastGesture = "";
int currentGesture;

int previousPosition = 0;
int currentPosition = 0;

public void setup() {
  
  size(600, 600);
  textSize(40);
    
  leap = new LeapMotionP5(this);
  port = new Serial(this, Serial.list()[0], 9600);
  
}

public void draw() {
  background(0);
  leap.enableGesture(Type.TYPE_SWIPE);
  leap.enableGesture(Type.TYPE_CIRCLE);
  
  for (Hand hand : leap.getHandList()) {
    ellipse(leap.getPosition(hand).x, leap.getPosition(hand).y, 20, 20);   
    
    // 0 - 24: 25 LEDs on my NeoPixel Strip
    currentPosition = (int) map(leap.getPosition(hand).x, 0, 1000, 0, 43);
    
    if (previousPosition != currentPosition) {
      previousPosition = currentPosition;
      port.write(previousPosition);
    }
   }
  fill(255);
  text(lastGesture, 200, 300);
}

public void swipeGestureRecognized(SwipeGesture gesture) { 
  // Display Gesture object data
    if (gesture.state() == State.STATE_STOP) {
      for (Hand hand : leap.getHandList()) {
        //Classify as right-left or up-down
          if (abs(leap.getDirection(hand).x) > abs(leap.getDirection(hand).y))  { 
            //Classify swipe as either horizontal or vertical
              if (leap.getDirection(hand).x > 0)  {
                  println("Right");
                  lastGesture = "Right" + "\n";
                  port.write(97);
              } 
              else {
                  println("Left");
                  lastGesture = "Left" + "\n";
                  port.write(98);
              }
          } 
          else { //vertical
              if (leap.getDirection(hand).y < 0)  {
                  println("Up");
                  lastGesture = "Up" + "\n";
                  port.write(99);
              } 
              else {
                  println("Down");
                  lastGesture = "Down" + "\n";
                  port.write(100);
              }                  
          }
     } 
    }
    else if (gesture.state() == State.STATE_START) {
    } 
    else if (gesture.state() == State.STATE_UPDATE) {
    }
}

public void circleGestureRecognized(CircleGesture gesture, String clockwiseness) { 
  if (gesture.state() == State.STATE_STOP) {
    if (clockwiseness == "clockwise" && gesture.durationSeconds() > 0.25)  {
      println("Clockwise");
      lastGesture = "Clockwise" + "\n";
      port.write(101);
     } 
  } 
  else if (gesture.state() == State.STATE_START) {
  } 
  else if (gesture.state() == State.STATE_UPDATE) {
  }
}

public void stop() {
  leap.stop();
}
