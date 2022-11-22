import {ConstantsStatic, YogaConfig} from './yoga';
export type * from './';

declare const mod: ConstantsStatic & {
    Config: {
        create: (buffer: BufferSource) => Promise<YogaConfig>;
    };
};

export default mod;
