# padamose

Padamose is a versioned in-memory store. It is part of the Volition blockchain project.

The versioned store is a simple database that can be efficiently rewound and branched. Its use case is writing blockchain applications where a complex state representing the composition of all ledger transactions needs to be maintained while at the same time considering revised and branched versions of the ledger.

This is an initial proof-of-concept implementation as an in-memory data structure. It is meant to be sufficient for prototyping, with single-session persistence and nothing more.

There is still a long way to go to get to beta:

Drivers for at least one (and ideally more) database backend need to be written. The most likely initial backend candidate is Redis. The goals for doing this will be to allow the version store to be persisted and to allow a large portion of the store to be moved out of application memory and to a server.

In implementing the drivers, the current implementation will need to be mostly rewritten. To the extent the current implementation remains, more efficient alternatives to STL map may need to be found or implemented.

Reference documentation:

https://cryptogogue.github.io/padamose/
