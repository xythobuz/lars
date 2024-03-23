$fa=1/1;
$fs=1/2;
bissl=1/100;
part="inner";//[inner,outer,all_visualize]
od_tamb=251;//tambourine rim outer diameter
id_tamb=239;//tambourine rim inner diameter
wall_tamb=(od_tamb-id_tamb)/2; //tambourine wall
hole_tamb=23; //holding hole in the rim
lip=3; //extra plastic that hols to the hole
air=1; //wiggle room
hole=3; //diameter for screw hole
beam_width=7; //slit in inner part of mount to hod the beam
wall=1.6;//thinnest part in the mount, namely the slit to tune the beam mounting height
module inner_mount() {
  difference() {
    union() {
      intersection() {
        cylinder(d=hole_tamb+2*lip-2*air,h=2*lip+wall_tamb);
        translate([0,0,-id_tamb/2+2*lip])rotate([90,0,0])cylinder(d=id_tamb,h=hole_tamb+2*lip,center=true);
      }
      intersection() {
        cylinder(d=hole_tamb-2*air,h=2*lip+wall_tamb);
        translate([0,0,-id_tamb/2+2*lip])rotate([90,0,0])cylinder(d=id_tamb+wall_tamb-air,h=hole_tamb+2*lip,center=true);
      }
    }
    hull() {
      translate([0,hole_tamb/2-air-hole/2-wall,-bissl])cylinder(h=wall_tamb+3*lip+2*bissl,d=hole);
      translate([0,-hole_tamb/2+air+hole/2+wall,-bissl])cylinder(h=wall_tamb+3*lip+2*bissl,d=hole);
    }
    translate([-beam_width/2-air/2,-hole_tamb/2-lip,-bissl]) cube([beam_width+air,hole_tamb+2*lip,lip]);
  }
}
module outer_mount() {
  difference() {
    union() {
      difference(){
        cylinder(d=hole_tamb+2*lip-2*air,h=2*lip+wall_tamb);
        translate([0,0,od_tamb/2+lip])rotate([90,0,0])cylinder(d=od_tamb,h=hole_tamb+2*lip,center=true);
      }
      difference(){
        cylinder(d=hole_tamb-2*air,h=2*lip+wall_tamb);
        translate([0,0,od_tamb/2+lip])rotate([90,0,0])cylinder(d=od_tamb-wall_tamb+air,h=hole_tamb+2*lip,center=true);
      }
    }
    hull() {
      translate([0,hole_tamb/2-air-hole/2-wall,-bissl])cylinder(h=wall_tamb+3*lip+2*bissl,d=hole);
      translate([0,-hole_tamb/2+air+hole/2+wall,-bissl])cylinder(h=wall_tamb+3*lip+2*bissl,d=hole);
    }
  }
}
if (part=="inner") inner_mount();
if (part=="outer") outer_mount();
if (part=="all_visualize") {
  inner_mount();
  translate([0,0,wall_tamb+3*lip])rotate([180,0,0])outer_mount();
}