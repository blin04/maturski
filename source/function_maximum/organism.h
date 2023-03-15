
class Organism {
    private:
    public:
        // 32-bit number - encoded using Gray's code
        unsigned int DNA;

        int getValue() {
            // output number that is encoded by DNA 

            int b = 0;

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