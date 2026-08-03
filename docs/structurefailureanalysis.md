# Structural Failure Analysis: Shoulder Joint and Forearm Truss at 1000 g
 
While testing the rover at a 1000 g payload — 200% of its 0.5 kg design target — I encountered two separate structural failures that took the arm out of service. This is a record of both: what failed, why it failed, and what I changed to fix the root cause.
 
The two are worth reading together because they failed for different reasons. The first was a build failure — the design was sound, but the as-built part didn't match it. The second was a design failure — the part was built as intended, but my model of how it would be loaded was incomplete. One taught me to build to spec; the other taught me to question my load assumptions.
 
---
 
## Failure #1: Shoulder–Elbow Joint
 
<!-- MEDIA: failure video — drag-and-drop the 1000 g joint-failure clip here on GitHub, or replace the line below -->
![Shoulder–elbow joint failing during the first 1000 g test](media/shoulder-joint-failure-video)
*The joint giving way during the first 1000 g payload test.*
 
<!-- MEDIA: photo of the deformed part -->
![Deformed shoulder–elbow mounting plate](media/shoulder-joint-deformed-part.jpg)
*The mounting plate after failure — the screw holes deformed and tore out under load.*
 
This failure appeared the first time I tested the 1000 g payload. The mounting plate on the shoulder–elbow link — the plate that bolts to the shoulder servo horn and carries the entire arm — deformed and pulled apart at the screw holes under the accumulated stress of repeated payload tests.
 
**Root causes:**
 
- **Prototype print settings.** The link holding up the whole arm had been printed at 2 walls and 15% infill — settings that are fine for prototyping but not for carrying large loads. With too few perimeters around the screw bosses, the holes had little material to resist the load and began to deform and elongate until they tore through.
- **Too few fasteners.** I had used two fasteners at the joint instead of the four the design called for, because two allowed faster disassembly during iteration. That forced the entire arm load through two screws, which was inadequate under heavy payload.
- **Hand-drilled center hole.** The center hole that lets the plate sit flush on the servo horn had been drilled with a power tool after printing rather than modeled into the part. This left a weak spot at the center of the plate and disrupted the surrounding material, which contributed to the screw holes deforming and failing.
**Fixes, each addressing a specific cause:**
 
- **Print settings → 3 walls, 25% infill.** The extra perimeters put more solid material around the screw bosses, where the load actually transfers, giving the holes far more resistance to deformation.
- **Fasteners → four, with washers and preload.** Going back to the full four-fastener pattern with washers spreads the load across the joint instead of concentrating it on two screws. I also preloaded the bolts more firmly: clamping the plate against the servo horn lets the joint carry load through friction across the whole mating face, taking stress off the screw holes themselves rather than relying on the screws in shear alone.
- **Center hole → designed into the print.** Modeling the hole into the part kept the surrounding material continuous and the plate's structural integrity intact.
**Result:** these changes fixed the joint. The reprinted link has passed several further 1000 g tests without deformation.
 
---
 
## Failure #2: Elbow–Wrist Forearm Truss
 
<!-- MEDIA: photo of the twisted/deformed truss -->
![Twisted elbow–wrist forearm truss](media/forearm-truss-deformed-part.jpg)
*The single-crossmember truss after it twisted under an off-axis load.*
 
This failure appeared during a demonstration of the rover lifting 1000 g. Mid-lift, the elbow–wrist link twisted along its length and deformed, and the arm failed.
 
**Root cause:**
 
I had designed the elbow–wrist link to be highly weight-optimized — strong where it needed to be, light everywhere else. Working from that mindset, I gave the top and bottom faces of the truss only a single crossmember each, because I expected the link to carry load only vertically, down the side faces. That reasoning held for a purely vertical load. But when the rover lifts a payload that isn't perfectly balanced, or when the arm accelerates in yaw, the load comes off the vertical axis and tries to twist the link along its length. With only one crossmember per face, the top and bottom behaved as an **open section**, which has very little torsional stiffness — so under that off-axis load, the truss twisted rather than holding its shape.
 
**Fix:**
 
<!-- MEDIA: render or photo of the two-crossmember redesign -->
![Redesigned two-crossmember truss](media/forearm-truss-redesign.png)
*The redesign — a second crossmember on each face closes the section toward a box.*
 
I redesigned the truss with the same weight-optimization mindset but a corrected understanding of how it would actually be loaded. The change was to run **two crossmembers per face instead of one**. Tying the top and bottom faces together this way closes the section further — better approximating a true closed section, which resists torsion far better than an open one — while keeping the lightweight advantage of a truss over a solid beam. The realization behind the fix wasn't "add more material"; it was that I had modeled only the in-plane bending load and missed the torsional load path entirely.
 
**Result:** the redesigned truss has passed every 1000 g test since, and it holds its shape far better when the arm is loaded during dynamic movement.
 
---
 
## Takeaway
 
Both failures came from deliberately testing past the design requirement, which is what that testing is for — it found the real limits of the build. The shoulder joint failed because the as-built part drifted from its design under the pressure of rapid iteration; the fix was discipline in fabrication. The forearm truss failed because the design itself was built on an incomplete load model; the fix was a better understanding of the loads. Together they moved the arm from meeting its 0.5 kg target to holding 1000 g repeatably.
