$fa=1/1;
$fs=1/2;
bissl=1/100;
wall=1.6;//twice nozzle diameter is fine, walls of triangles
hole=3; //hole for mounting thread inserts
hd=10; //horizontal distance between mounting points, depends on mounting hole distance in the actuator
height=40; //dimension of the beam in the direction of tambourine depth
vd=height-wall; //vertical distance between mounting holes
segments=14; //number of mounting points, defines the beam length
radius=1; //rounding of inner corners
thickness=2*wall+hole; //thickness of the beam, keep some material to properly hold the threaded insert on the side
offset=wall/2+hole/2;
difference() {
  linear_extrude(thickness, convexity=4) difference() {
    offset(r=-radius)offset(r=radius)union() {
      for (i=[0:hd:hd*(segments-1)]) translate ([i,0]) {
        for (tr=[[[hd,0],[0,0]],[[-hd/2,vd],[0,0]],[[hd/2,vd],[0,0]],[[hd/2,vd],[-hd/2,vd]]]) hull() { //diagonals
          translate(tr[0])circle(d=wall);
          translate(tr[1]) circle(d=wall);
        }
        for (tr=[[hd/2,vd-offset],[-hd/2,vd-offset],[hd,offset],[0,offset]]) translate(tr)circle(d=2*wall+hole); //big circles
      }
    
      //left vertical wall, not generated by the loop
      for (tr=[[[-hd/2-offset,0],[0,0]],[[-hd/2-offset,0],[-hd/2-offset,vd]],[[-hd/2-offset,vd],[-hd/2,vd]]]) hull() {
        translate(tr[0])circle(d=wall);
        translate(tr[1]) circle(d=wall);
      }

      //solid triangle to jhold threaded inserts
      hull() for (tr=[[0,0],[-hd/2-offset,0],[-hd/2-offset,vd],[-hd/2,vd]]) translate(tr) circle(d=wall); 

      //right vertical wall, not generated by the loop
      for (tr=[[[hd*segments+offset,0],[0,0]],
               [[hd*segments+offset,0],[hd*segments+offset,vd]],
               [[hd*segments+offset,vd],[hd*(segments-0.5),vd]],
               [[hd*segments,0],[hd*(segments-0.5),vd]]]) 
        hull() {
          translate(tr[0])circle(d=wall);
          translate(tr[1]) circle(d=wall);
        }
    }
    for (i=[0:segments]) {
      translate([i*hd,offset]) circle(d=hole);
      translate([(i-0.5)*hd,vd-offset]) circle(d=hole);
    }
  }
  //mounting holes for threaded inserts, three just in case, it's better to use the middle one
  translate([-hd/2-offset-wall/2,height/4-wall,thickness/2])rotate([0,90,0])cylinder(d=hole,h=hd-wall);
  translate([-hd/2-offset-wall/2,height/2-wall/2,thickness/2])rotate([0,90,0])cylinder(d=hole,h=hd-wall);
  translate([-hd/2-offset-wall/2,height*3/4,thickness/2])rotate([0,90,0])cylinder(d=hole,h=hd-wall);
}