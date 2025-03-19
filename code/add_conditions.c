#include <stddef.h>
#include "cnf.h"

//
// LOGIN: xurbana00
//

// Tato funkce by mela do formule pridat klauzule predstavujici podminku 1)
// Křižovatky jsou reprezentovany cisly 0, 1, ..., num_of_crossroads-1
// Cislo num_of_streets predstavuje pocet ulic a proto i pocet kroku cesty
// Pole streets ma velikost num_of_streets a obsahuje vsechny existujuci ulice
//    - pro 0 <= i < num_of_streets predstavuje streets[i] jednu existujici
//      ulici od krizovatky streets[i].crossroad_from ke krizovatce streets[i].crossroad_to
void at_least_one_valid_street_for_each_step(CNF* formula, unsigned num_of_crossroads, unsigned num_of_streets, const Street* streets) {
    assert(formula != NULL);
    assert(num_of_crossroads > 0);
    assert(num_of_streets > 0);
    assert(streets != NULL);

    for (unsigned i = 0; i < num_of_streets; ++i) {
        Clause* cl = create_new_clause(formula);
        for (unsigned j = 0; j < num_of_streets; ++j) {
            add_literal_to_clause(cl, true, i, streets[j].crossroad_from, streets[j].crossroad_to);
        }
    }
}

// Tato funkce by mela do formule pridat klauzule predstavujici podminku 2)
// Křižovatky jsou reprezentovany cisly 0, 1, ..., num_of_crossroads-1
// Cislo num_of_streets predstavuje pocet ulic a proto i pocet kroku cesty
void at_most_one_street_for_each_step(CNF* formula, unsigned num_of_crossroads, unsigned num_of_streets) {
    assert(formula != NULL);
    assert(num_of_crossroads > 0);
    assert(num_of_streets > 0);

    for (unsigned i = 0; i < num_of_streets; i++) {
        // pro kazdy krok i
        for (int u11 = 0; u11 < num_of_crossroads; u11++) {
            for (int u12 = 0; u12 < num_of_crossroads; u12++) {
                for (int u21 = 0; u21 < num_of_crossroads; u21++) {
                    for (int u22 = 0; u22 < num_of_crossroads; u22++) {
                        // pro kazdou kombinaci ulic
                        if (u11 != u21 || u12 != u22) {
                            // ulice nejsou stejne
                            Clause *cl = create_new_clause(formula);
                            add_literal_to_clause(cl, true, i, u11, u12);
                            add_literal_to_clause(cl, true, i, u21, u22);
                        }
                    }
                }
            }
        }
    }
}

// Tato funkce by mela do formule pridat klauzule predstavujici podminku 3)
// Křižovatky jsou reprezentovany cisly 0, 1, ..., num_of_crossroads-1
// Cislo num_of_streets predstavuje pocet ulic a proto i pocet kroku cesty
void streets_connected(CNF* formula, unsigned num_of_crossroads, unsigned num_of_streets) {
    assert(formula != NULL);
    assert(num_of_crossroads > 0);
    assert(num_of_streets > 0);

    for (unsigned i = 0; i < num_of_streets-1; ++i) {
        // pro kazdy krok i
        for (unsigned z = 0; z < num_of_crossroads; ++z) {
            for (unsigned k1 = 0; k1 < num_of_crossroads; ++k1) {
                // pro kazde (z,k1)
                Clause *cl = create_new_clause(formula);
                add_literal_to_clause(cl, false, i, z, k1);
                for (unsigned k2 = 0; k2 < num_of_crossroads; ++k2) {
                    add_literal_to_clause(cl, true, i + 1, k1, k2);
                }
            }
        }
    }
}

// Tato funkce by mela do formule pridat klauzule predstavujici podminku 4)
// Křižovatky jsou reprezentovany cisly 0, 1, ..., num_of_crossroads-1
// Cislo num_of_streets predstavuje pocet ulic a proto i pocet kroku cesty
void streets_do_not_repeat(CNF* formula, unsigned num_of_crossroads, unsigned num_of_streets) {
    assert(formula != NULL);
    assert(num_of_crossroads > 0);
    assert(num_of_streets > 0);
    
    for (unsigned i = 0; i < num_of_streets; ++i) {
        // pro kazdy krok i
        for (unsigned j = 0; j < num_of_streets; ++j) {
            if (i != j) {
                // pro kazdy jiny krok j
                for (unsigned z = 0; z < num_of_crossroads; ++z) {
                    for (unsigned k = 0; k < num_of_crossroads; ++k) {
                        // pro kazdu dvojici krizovatek (z, k)
                        Clause* cl = create_new_clause(formula);
                        add_literal_to_clause(cl, false, i, z, k);
                        add_literal_to_clause(cl, false, j, z, k);
                    }
                }
            }
        }
    }
}
