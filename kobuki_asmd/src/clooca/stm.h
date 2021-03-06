
#ifndef D_MEXU_H_
#define D_MEXU_H_

#include "main.h"
#include "MEXU.h"


/* Event ID Definitions */

#define KobukiStateMachine_EVENT_BUTTON_PRESSED 0

#define KobukiStateMachine_EVENT_NEXT 1

#define KobukiStateMachine_EVENT_FINISH 2

#define KobukiStateMachine_EVENT_TURN_REACHED 3

#define KobukiStateMachine_EVENT_RUN_REACHED 4

#define KobukiStateMachine_EVENT_BUMPER_PRESSED 5

#define KobukiStateMachine_EVENT_WHEEL_DROP 6

#define KobukiStateMachine_EVENT_IGNORE 7



/*====================================================
 * KobukiStateMachine class
 *
 */

class KobukiStateMachine : public StateMachine {
private:
  static const int matrix[];
public:
  /* attributes */

  int visit_dock;

  Map map;

  bool endflg;

  /* methods */                          
  KobukiStateMachine() : StateMachine(){
      eventManager.addSTM(this);
      current_state = 0;

      init();
  }

  ~KobukiStateMachine() {}
  int execute(MEXUEvent *event);
  void action(int state);

  void init() {
      map.resize(8.0, 8.0);
    visit_dock = 0;
    endflg = false;
  }

  void setNextObstacle() {
      if( map.setNextUncheckedObstacle() ) goNext();
    else goFinish();
  }

  void setNextSide() {
      if( map.setNextSide() ) goNext();
    else goFinish();
  }

  void turnToObstacle() {
      map.turnToObstacle( 1.0 );
  }

  void runToObstacle() {
      map.turnToObstacle( 1.0 );
  }

  void measureBorder() {
      map.measureBorder();
  }

  void saveMap() {
      map.showIR();
      map.showMe();
    goFinish();
  }

  void setNextIRBlock() {
      if( map.setNextIRBlock() ) goNext();
    else{
      std::cout<<"IRBlock not found"<<std::endl;
      goFinish();
    }
  }

  void docking() {
    map.docking();
    map.markDockBlock( map.findDockBlock() );
    map.Wait( 4.0 );
    visit_dock++;
    goNext();
  }

  void checkDock() {
      if( visit_dock < 2 ){
      map.clearIRMark();
      map.goBackToCurrent( 0.3 );
      goNext();
    }
    else goFinish();
  }

  void foundObstacle() {
      map.foundObstacle();
    map.goBackToCurrent(0.3);
  }

  void mapping() {
      map.mappingBlank();
    map.recordIR();
    goNext();
  }

  void setNextBlock() {
      if( map.setPathToNextBlock() ) goNext();
    else goFinish();
  }

  void checkNext() {

    std::cout<< "next: " << map.next_block->getTagX() << "," << map.next_block->getTagY() << std::endl;
    map.showMe();
    map.updateCurrent();

    if( map.isReached() ){
      goFinish();
    }
    else {
      map.setNext();
      goNext();
    }
  }

  void turnToNext() {
      map.turnToNext( 1.0 );
  }

  void runToNext() {
      map.runToNext( 0.3 );
  }

  void initializeMap() {
    map.checkWall();
//  map.searchObstacleBlocks();
    goNext();
  }

  void goNext() {
      this->sendEvent( KobukiStateMachine_EVENT_NEXT );
  }

  void goFinish() {
      this->sendEvent( KobukiStateMachine_EVENT_FINISH );
  }

  void stop() {
  endflg = true;
  }

};
/* State ID Definitions */
#define KobukiStateMachine_STATE_IGNORE -1
  

#define KobukiStateMachine_STATE_start 0

#define KobukiStateMachine_STATE_mapping 1

#define KobukiStateMachine_STATE_setNextBlock 2

#define KobukiStateMachine_STATE_checkNext_1 3

#define KobukiStateMachine_STATE_turnToNext_1 4

#define KobukiStateMachine_STATE_runToNext_1 5

#define KobukiStateMachine_STATE_foundObstacle 6

#define KobukiStateMachine_STATE_initializeMap 7

#define KobukiStateMachine_STATE_saveMap 8

#define KobukiStateMachine_STATE_setNextObstacle 9

#define KobukiStateMachine_STATE_setNextSide 10

#define KobukiStateMachine_STATE_checkNext_2 11

#define KobukiStateMachine_STATE_turnToNext_2 12

#define KobukiStateMachine_STATE_runToNext_2 13

#define KobukiStateMachine_STATE_measureBorder 14

#define KobukiStateMachine_STATE_turnToObstacle 15

#define KobukiStateMachine_STATE_runToObstacle 16

#define KobukiStateMachine_STATE_setNextIRBlock 17

#define KobukiStateMachine_STATE_checkNext_3 18

#define KobukiStateMachine_STATE_turnToNext_3 19

#define KobukiStateMachine_STATE_runToNext_3 20

#define KobukiStateMachine_STATE_docking 21

#define KobukiStateMachine_STATE_checkDock 22

#define KobukiStateMachine_STATE_stop 23




#endif /* D_MEXU_H_ */
