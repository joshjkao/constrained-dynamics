# constraineddynamics

This program was developed as part of a project for Physics 304: Mechanics and is written in C++ using SDL2

For this project, I decided to analyze and visualize chaos in deterministic physical systems, particularly multiple pendulum systems. While the motion of a
single simple pendulum consisting of a point mass constrained to rotate around a point is regular and can be easily described with one equation, attaching 
more pendulums in a chain causes chaos to arise. That is, even small differences in initial conditions will eventually cause two closely related systems to 
diverge after a certain point.

To learn more about the phenomenon of chaos and the context of the double pendulum system, see the following papers:  
  [1] https://aip.scitation.org/doi/10.1063/5.0000582
  [2] https://arxiv.org/abs/1910.12610
  
While researching for this project, I found articles analyzing which changes to which initial conditions caused chaos to arise fastest. My first purpose 
with this program was to compare my results with theirs, but also to observe what happens when two different simulations are applied to the same initial 
conditions. After some testing, I found that the two simulations run with the same initial conditions behaved remarkably similarly (making it almost 
impossible to tell that multiple simulations were running), but after a few seconds would diverge into chaos just like two systems run on the same 
simulation with different initial conditions. More details will be released once the final paper is complete.

To accomplish this goal, I used two different types of simulations:
  1. Lagrangian Formalism of Mechanics
    As laid out in [1], the equations of motion for the double pendulum can be derived using the principle of least action, meaning that objects move in 
    such a way that this "action integral" is stationary over time. This method involves describing the system in terms of two generalized coordinates, 
    the angles of both pendulums to the vertical (this is because both point masses are constrained in the r direction as the pendulums are fixed length,
    so the system has 2 degrees of freedom). Following the process described in [1] yields two second-order differential equations for the second time 
    derivatives of both angles as a function of both position and velocity. Next, we can simply use a numerical solution to extrapolate and visualize how 
    these changes look over time.
    
  2. Force-based dynamics
    This approach is best explained by the following material: [3] https://www.cs.cmu.edu/~baraff/sigcourse/slidesf.pdf
    Instead of viewing the system in terms of generalized coordinates, this approach involves generalizing the problem even more. The double pendulum (or 
    any pendulum system) can be taken as a chain of point masses subject to a series of constraints. In order to extrapolate their motion, we can simply 
    apply all external forces (in this case only gravity) and then calculate the magnitude of each "constraint force" that is needed to satusfy each 
    constraint. In practice, each constraint takes the form of an equation that equals zero when it is satisfied. When there are multiple constraints and
    thus multiple constraint equations (such as our double or triple pendulum), they are combined into one large matrix equation so all constraint forces 
    can be satisfied at the same time. This involves taking the first and second time derivatives of the constraint matrix equation, and the use of 
    lagrange multipliers since we know each constraint force must be orthogonal to velocity. In the end, the matrix equation can be put into form Ax = B, 
    where A is a matrix of known values, B is a vector of known values, and x is the vector of the magnitudes of the constraint forces. In this program, 
    I used a matrix class implement all the linear algebra involved, including Gaussian Elimination to solve the final Ax = B equation. More sources 
    going into more detail about this approach are attached below.
    
    Note: ALL simulations were run at 60fps at 300 iterations per frame, for a total of 30,000 iterations per second with a timestep (dt) of 0.00005 sec.   
    The ODE numerical solver implements Euler's method, though I could also run more tests comparing different implementations of numerical solving 
    algorithms.
    

Helpful sources I used for the Force-based approach:
[4] https://www.toptal.com/game/video-game-physics-part-iii-constrained-rigid-body-simulation#:~:text=Constraints%20are%20essentially%20rules%20that,freedom%20from%20a%20rigid%20body.
[5] https://youtu.be/TtgS-b191V0
