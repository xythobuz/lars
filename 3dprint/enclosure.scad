//currantly the box is unfinished, but you can still use faceplate.
$fa=1/1;
$fs=1/2;
bissl=1/100;
part="bottom";//[bottom,top,faceplate]
pegs_h=5;
pegs_id=3;
pegs_od=7;
wall=1;
depth=30;
air=0.5; //wiggle room
//board (and box) size goes here
//defined as corners of inner space relatove to x=y=0 point on the board
//[x1,y1,x2,y2]
board=[-2,-2,90.81+4,118.75+2];
screen=[67.2+2,16.5-10,20,27];
//holes in the front panel
//defined as [x,y,diameter], where x from KiCAD becomes y and y becomes x, because
//coordinate conventions in Kicad and OpenSCAD are mirrored
holes_positions=[
  [48.90,66.68,12], //encoder
  [48.3,85.4,7], //power
  [71.12,48.26,8], //leds
  [71.12,67.31,8],
  [71.12,86.36,8],
  [71.12,105.41,8],
  [85.73,48,12], //buttons
  [85.73,67.3,12],
  [85.73,86.36,12],
  [85.73,105.41,12]];
//standoffs to hold the board
//defined as [x,y,id,od,height], where x from KiCAD becomes y and y becomes x, because
//coordinate conventions in Kicad and OpenSCAD are mirrored
mount_pegs=[
  [1.90,1.90,pegs_id,pegs_od,pegs_h],
  [1.90,116.84,pegs_id,pegs_od,pegs_h],
  [88.90,1.90,pegs_id,pegs_od,pegs_h],
  [88.90,116.84,pegs_id,pegs_od,pegs_h]
];
module bottom() {
  difference() {
    translate([board[0]-wall,board[1]-wall,-wall]) cube([board[2]-board[0]+2*wall,board[3]-board[1]+2*wall,depth+wall]);
    translate([board[0],board[1],0]) cube([board[2]-board[0],board[3]-board[1],depth+bissl]);
  }
  for (peg=mount_pegs) {
    translate([peg[0],peg[1],0]) difference() {
      cylinder(d=peg[3],h=peg[4]);
      translate([0,0,-bissl]) cylinder(d=peg[2],h=peg[4]+2*bissl);
    }
  }
}
module faceplate() {
  difference() {
    translate([board[0]-wall,board[1]-wall,0]) cube([board[2]-board[0]+2*wall,board[3]-board[1]+2*wall,wall]);
    for (hole=holes_positions) {
      translate([hole[0],hole[1],-bissl]) cylinder(d=hole[2],h=wall+2*bissl);
    }
    translate([screen[0],screen[1],-bissl])cube([screen[2],screen[3],wall+2*bissl]);
  }
};
module top() {
  mirror([0,0,1])faceplate();
    difference() {
    translate([board[0]-wall,board[1]-wall,0]) cube([board[2]-board[0]+2*wall,board[3]-board[1]+2*wall,depth]);
    translate([board[0],board[1],-bissl]) cube([board[2]-board[0],board[3]-board[1],depth+2*bissl]);
  }
}
if (part=="bottom") bottom();
if (part=="faceplate") faceplate();
if (part=="top") top();
if (part=="all") {
  bottom();
  translate([0,0,depth+wall]) faceplate();
}