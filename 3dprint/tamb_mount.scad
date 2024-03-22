$fa=1/1;
$fs=1/2;
bissl=1/100;
part="inner";//[inner,outer,all_visualize]
od_tamb=251;
id_tamb=239;
wall_tamb=(od_tamb-id_tamb)/2;
hole_tamb=23;
lip=3;
air=1;
hole=3;
wall=1.6;
beam_width=7;
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