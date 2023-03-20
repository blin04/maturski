
class Organism {
    private:

    public:
        // binary number written in memory
        unsigned int DNA;

        // number encoded by DNA
        unsigned int getValue() {
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
};