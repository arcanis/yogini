import Yoga from 'yoga-layout';
export type * from 'yoga-layout';

declare const mod: {
    (buffer: BufferSource): typeof Yoga;
};

export default mod;
