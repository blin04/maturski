
class Organism {
    private:

    public:
        unsigned int DNA;

        bool operator == (Organism o) {
            return DNA == o.DNA;
        }

        unsigned int getValue() {
            /* returns value of DNA, which is coded in gray's code */
            unsigned int b = 0;
            int bit;

            for (int j = 31; j >= 0; j--) {
                bit = (DNA & (1 << j) ? 1 : 0);
                if (j != 31) {
                    bit ^= (DNA & (1 << (j + 1)) ? 1 : 0);
                }

                if (bit) b |= (1 << j);
            }
            
            return b;
        }

        double getEncodedNumber(double A, double B) {
            /* returns real number that is encoded by this chromosome
            in interval [A, B] */ 

            double x = (getValue()) / (UINT_MAX - 1.0);
            return A + x*(B - A);
        }
};