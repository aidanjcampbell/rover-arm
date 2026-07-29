# Servo Fault Isolation

## The symptom

I installed a new controller board and wired it to the arm. On power-up, the board's status light came on, but the connected servo would not respond to any command. No movement. No error message. No partial or intermittent behavior. Complete silence.

## Why silence is the hard case

Most bugs give you a thread to pull. A wrong number, an error message, a behavior that's *almost* right. Leads like these allow you to focus in on a specific hardware piece or section of code. But in this situation, I had nothing to initially focus my attention to.

To work around this, I decided to look at the broad picture of the interactions taking place that would make this servo eventually move. By dividing everything into systems, first hardware/software and then further, I was able to isolate areas of possible failure and test them independently.
## The initial signal

While the servo was not functioning, there was a status light visible inside the servo housing. This led me to believe that the servo was at least receiving power and possibly commands, focusing my attention towards software first.

## Mapping the possibilities

Before touching anything, I listed every layer that could produce total silence:

- my control software
- the communication settings (speed, wiring orientation, device addresses)
- the physical wiring
- a defective motor
- a defect on the board itself

Here is my full problem diagnosis tree:


To efficiently work through this tree and eliminate as many potential problems as possible while following the hint the status light gave me, I decided to start on the software side of the tree. In doing this I was able to eliminate any potential software bugs before I tried replacing hardware, which costs time and money. 

## Shrinking the problem

The full firmware ran a web interface, motor control, and a wireless connection layered on top of the motor communication. Any of those could have been interfering. So I set all of it aside and wrote a bare test program that did nothing but talk to the motors.

This was the core move: hold everything constant, remove one thing. If the motors worked in the stripped-down program, the fault lived somewhere in the rest of the system. If they still didn't, I'd eliminated an entire category of suspects in a single step.

With that minimal test I confirmed, one check at a time: the board had power, the software was the correct version, the wiring matched the manufacturer's own reference exactly, every valid orientation of the communication wires was tried, and I scanned the full range of possible device addresses at every possible communication speed. Each check closed off a branch. None produced a response.

## The remaining three

That left three possibilities I could not tell apart from software:

1. a defective motor
2. a defect on the board
3. a defective wire

These three were physically separable, and I could state in advance what each test would prove:

- **Another servo on the same board.** If it works, the first servo was the problem. If it doesn't, the servo isn't the issue.
- **Another wire with the same setup.** If it works, the first wire was the problem. If it doesn't, that leaves only the board on my decision tree
- **The same setup on a second board.** If it works, the original board was defective. If it doesn't, the fault is somewhere I haven't looked yet or with the design of the board itself, testable with a bare-bones ervo controller.

Stating the expected outcome *before* running the test is what separates a diagnosis from swapping parts and hoping something changes.

## Resolution

First testing both the new servo and new wire (the servo and wire to be used on the elbow joint), they both produced no result, eliminatng them from the tree.

I then moved the identical code and hardware steup onto a replacement board. The motors responded immediately.

Nothing in my software, wiring, or configuration had changed — so the one thing that *did* change was the fault. The original board had a defective communication circuit: either a factory defect or a component I damaged during the development process, either of which would have gone unnoticed as all other functions worked.

## Why the diagnosis mattered

The fix was trivial: swap a board. That's worth being honest about — the value here was never the fix.

The days went into *earning* the conclusion. A hardware defect is the one root cause that can't be measured directly; no test ever reads "board faulty." It can only be reached by eliminating everything under my control first. Blaming the hardware before that point is a guess; blaming it after is a diagnosis. The discipline was refusing the comfortable assumption — that the new part was fine and the fault was mine — until I had proven where the fault was not.

## Principles

1. When a system is completely silent, there's no signal to follow — shrink the space instead: hold everything constant, remove one variable at a time, until the cause is cornered.
2. A hardware fault is only a credible conclusion after the controllable parts are exhausted, never before. Reaching for it early feels like progress and usually isn't.
