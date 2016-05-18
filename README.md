# AliDPG
ALICE DPG SW Repository

# Proposed branching structure

This is a summary of the naming discussion between Roberto, Chiara and Giulio about how to name / release AliDPG. This is of course subject to changes and amendments.

- For each AliRoot / AliPhysics release  `vA-B-C` / `vA-B-C-D` there will be a single AliDPG branch `vA-B-XX` which is valid for all the AliRoot / AliPhysics matching `vA-B-.*` released up to that point.
- An AliDPG release is always cut from the tip of one of the `vA-B-XX` branches and is guaranteed to work with all the AliRoot / AliPhysics matching `vA-B-.*` released up to that point. Every time a build of AliDPG is asked, it will get a package named `vA-B-XX-<N>` where <N> indicate the number of rebuild. The user should always pick up the release with the highest build number, given a branch.
