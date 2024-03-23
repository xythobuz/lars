$fa=1/1;
$fs=1/2;
bissl=1/2;
part="spool";//[spool,cap,hammer,all]
total_length=50;
wall=0.8;
air=0.5;
copper_height=5;
thick_wall=2*wall; //can be specified
iron_d=4;
iron_h=total_length/3; //can be changed, but this seems optimal
spool_h=iron_h; //according to theory, optimum is a little more
iron_travel=iron_h; //may be smaller with heavy beaters or returning springs
extra_hold=iron_h/3;
spool_id=iron_d+2*air;
spool_od=spool_id+2*wall;
cap_od=spool_od+2*copper_height;
slit=15;
hole=3;
hammer_flat=true;
module spool() {
  difference() {
    union() {
      cylinder(d1=spool_od,d2=cap_od,h=thick_wall+iron_travel);
      translate([0,0,iron_travel+thick_wall]) cylinder(d=spool_od,h=spool_h+extra_hold);
    }
    translate([0,0,thick_wall]) cylinder(d=spool_id,h=spool_h+iron_h+bissl+extra_hold);
  }
}
module cap() {
difference() {
union() {
cylinder(d=cap_od,h=thick_wall);
translate([0,0,thick_wall])cylinder(d=spool_od+2*wall+2*air,h=extra_hold);
translate([-cap_od/2,0,0])cube([cap_od,cap_od/2,thick_wall]);
translate([-cap_od/2,cap_od/2,0])cube([cap_od,thick_wall,2*thick_wall+slit]);
translate([-thick_wall/2,0,0])cube([thick_wall,cap_od/2,extra_hold+thick_wall]);
}
translate([0,0,-bissl])cylinder(d=spool_id,h=extra_hold+thick_wall+2*bissl);
cylinder(d=spool_od+2*air,h=extra_hold);
hull() {
translate([cap_od/2-thick_wall-hole/2,cap_od/2+thick_wall/2,2*thick_wall+hole/2])rotate([90,0,0])cylinder (d=hole, h=thick_wall+bissl,center=true);
translate([cap_od/2-thick_wall-hole/2,cap_od/2+thick_wall/2,slit])rotate([90,0,0])cylinder (d=hole, h=thick_wall+bissl,center=true);
}
hull() {
translate([-cap_od/2+thick_wall+hole/2,cap_od/2+thick_wall/2,2*thick_wall+hole/2])rotate([90,0,0])cylinder (d=hole, h=thick_wall+bissl,center=true);
translate([-cap_od/2+thick_wall+hole/2,cap_od/2+thick_wall/2,slit])rotate([90,0,0])cylinder (d=hole, h=thick_wall+bissl,center=true);
}
}
}
echo (cap_od-thick_wall*2-hole);
module hammer() {
if (hammer_flat) {
cylinder(d=cap_od-2*air,h=thick_wall);
translate([0,0,thick_wall]) cylinder(d1=cap_od-2*air,d2=iron_d,h=cap_od/2-iron_d/2-air/2);
}
else {
cylinder(d2=cap_od/2,d1=iron_d,h=cap_od/4-iron_d/4);
translate([0,0,cap_od/4-iron_d/4])cylinder(d=cap_od/2,h=thick_wall);
translate([0,0,cap_od/4-iron_d/4+thick_wall]) cylinder(d1=cap_od/2,d2=iron_d,h=cap_od/4-iron_d/4);
}
translate([0,0,thick_wall+cap_od/2-iron_d/2-air/2]) cylinder(d=iron_d,h=1.5*iron_h);
}
if (part=="all") {
spool();
translate([0,0,spool_h+iron_h+bissl+air+thick_wall])cap();
translate([0,0,total_length])mirror([0,0,1])hammer();
}
if (part=="spool") spool();
if (part=="cap") cap();
if (part=="hammer") hammer();
