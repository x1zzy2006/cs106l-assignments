/*
 * CS106L Assignment 3: Make a Class
 * Created by Fabio Ibanez with modifications by Jacob Roberts-Baca.
 */

 #include <iostream>
 #include <string>
 #include "class.h"

void sandbox() {
  // STUDENT TODO: Construct an instance of your class!
  teacher me = teacher();
  teacher bro = teacher("trolling", 0, 0);
  bro.fuckFace('o');
  // bro.setFace(); private看调用位置而非调用者

  bro.setRank(1);
  std::cout << bro.getRank() << std::endl;
}