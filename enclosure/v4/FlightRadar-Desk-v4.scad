// FlightRadar E32R35T desk enclosure v4. Dimensions in millimeters.
part = "assembly"; // [assembly,bezel,shell,fit-check]
usb_y = 0;
usb_z = 8.8;
plug_width = 15;
plug_height = 9;
$fn = 48;

seam = 9;
depth = 32;
tilt = 21;
function foot(z) = -33.4 + z*tan(tilt);

module rounded_rectangle(w,h,r) {
    offset(r=r) square([w-2*r,h-2*r],center=true);
}
module layer(z,w,low,high,r) {
    translate([0,(low+high)/2,z]) linear_extrude(.04)
        rounded_rectangle(w,high-low,r);
}
module profile(sections) {
    for (i=[0:len(sections)-2])
        hull() {
            for (j=[i,i+1]) {
                p=sections[j];
                layer(p[0],p[1],p[2],p[3],p[4]);
            }
        }
}
module slab(z,w,h,r,d) {
    translate([0,0,z]) linear_extrude(d) rounded_rectangle(w,h,r);
}
module board_mounts() {
    for(x=[-47.25,47.25],y=[-23.95,23.95]) translate([x,y,0]) children();
}
module shell_mounts() {
    for(x=[-29,29],y=[-29.2,29.1]) translate([x,y,0]) children();
}
module usb_relief() {
    translate([54,usb_y,usb_z]) rotate([0,90,0])
        linear_extrude(17,center=true) rounded_rectangle(plug_height,plug_width,1.8);
}
module bezel() {
    difference() {
        profile([
            [0,93.0,-32.2,28.55,4.7],
            [1.2,98.4,-33.0,29.6,5.2],
            [3.2,105.8,-33.1,30.6,6.2],
            [5.8,108.3,-32.0,31.3,6.7],
            [seam-.04,108.6,foot(seam),31.5,6.8]
        ]);
        hull() {
            layer(-.1,79.4,-26.6,26.6,1.8);
            layer(1.52,75.4,-25.25,25.25,.7);
        }
        slab(1.49,75.4,50.5,.7,10);
        slab(1.8,86,56.5,1.4,4.25);
        slab(6,102.5,56.5,3.7,4);
        board_mounts() translate([0,0,2.05]) cylinder(d=2.1,h=7.1);
        shell_mounts() translate([0,0,3.5]) cylinder(d=2.2,h=7);
        usb_relief();
        translate([0,-33.4,0]) rotate([-tilt,0,0])
            translate([-65,-50,-10]) cube([130,50,70]);
    }
}
module rear_skin() {
    profile([
        [seam,108.6,foot(seam),31.5,6.8],
        [11.5,108.4,foot(11.5),31.1,6.9],
        [14.5,107.1,foot(14.5),29.1,7.1],
        [17.8,104.7,foot(17.8),24.0,7.4],
        [21.3,102.0,foot(21.3),15.4,7.7],
        [25.7,100.2,foot(25.7),5.5,7.8],
        [29.2,98.7,foot(29.2),-.5,7.2],
        [31.0,96.0,foot(31.0)+.35,-3.2,6.6],
        [depth-.04,92.8,foot(depth)+1.0,-5.0,5.8]
    ]);
}
module rear_cavity() {
    profile([
        [seam-.15,103.3,foot(seam)+2.2,29.0,4.1],
        [11.5,103.1,foot(11.5)+2.5,28.5,4.3],
        [14.5,101.5,foot(14.5)+2.5,25.9,4.5],
        [17.8,99.0,foot(17.8)+2.5,19.7,4.6],
        [21.3,96.3,foot(21.3)+2.5,10.2,4.7],
        [25.7,94.5,foot(25.7)+2.5,1.2,4.5],
        [29.3,92.0,foot(29.3)+2.8,-5.5,4.0]
    ]);
}
module shell() {
    difference() {
        union() {
            difference() { rear_skin(); rear_cavity(); }
            intersection() {
                rear_skin();
                union() shell_mounts() translate([0,0,seam]) cylinder(d=8.3,h=5);
            }
        }
        shell_mounts() {
            translate([0,0,seam-.05]) cylinder(d=3,h=5.1);
            translate([0,0,seam+4]) cylinder(d=5.6,h=32);
        }
        usb_relief();
        for(x=[-18:6:18]) translate([x,11,0]) slab(14,2.3,8,1.12,12);
    }
}
module fit_check() {
    intersection() {
        bezel();
        union() {
            translate([-51,-33,-1]) cube([104,4,12]);
            translate([39,-13,-1]) cube([20,26,12]);
            for(x=[-47,47]) {
                translate([x-2,-33,-1]) cube([4,55,12]);
                for(y=[-24,24]) translate([x-7,y-7,-1]) cube([14,14,12]);
            }
        }
    }
}
mirror([1,0,0]) {
    if (part == "bezel") bezel();
    else if (part == "shell") translate([0,0,-seam]) shell();
    else if (part == "fit-check") fit_check();
    else {
        color([.22,.24,.27]) bezel();
        color([.18,.20,.23]) shell();
        %slab(1.8,84.36,55,1.5,.4);
    }
}
