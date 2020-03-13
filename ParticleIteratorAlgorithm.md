# Particle Iterator Algorithm

Each particle species (PDG code or charge) has its own ParticleIter object.

These are linked in series for the different species.


Iterators later in the series are classed as Inner and are iterated first. These are refereed to as InnerParticle

The highest level InnerParticle is just responsible for selecting the N required particles of that species from the M in the particle species vector (i.e. the EventParticles->GetParticleVector()


Within each particle species iterator, InnerParticle,  there may be several inner iterators used to achieve the correct interative behaviour. These are refered to as InnerSelect

The InnerSelect use a combitorial selection procedure for selecting the particles in order. i.e. Select X of Y

To an  InnerSelect you may add a further InnerSelect to the selected X particles or the remaining Y-X particles.

For example, my InnerParticle(A) may select 4 gammas from M gammas in the event; its InnerSelect(B)  may then select 2 of these gammas (2 from 4, to make a pi0); its InnerSelect(C) may then select the remaining 2 gammas (2 from 2, another pi0)

1 2 3 4 5 6
A 1234
B 12 C 34
A 1235
B 12 C 35

For example, my InnerParticle(A) may select 3 +ve from M +ve in the event; its InnerSelector(P) may then select 1 proton from these (1 from 3); its  InnerSelect(C) may then select the remaining 2 +ve to be pi+.

Note in this case if there are 4 +ve particles, number 3 from 4 = 4, number 1 from 3 = 3. Total combinations = 3*4=12

If 1 of the FinaState class pi+ has a parent (e.g. an omega meson) then we need to try both combinations of pi+ as the child. Here we then need to select from the 2 pions:
 The  InnerSelect(D) of (C) may then select 1 pi+ to have a parent select the remaining one to be included but independent, InnerSelect(E).



Once an inner iterator(Q) is complete it moves back to the preceding inner iterator(P) for the next event. After the next event it moves back to the inner (Q) and completes another cycle of Q.

