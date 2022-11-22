import {ConstantsStatic, YogaConfig} from './yoga';
export type * from './yoga';

declare const mod: ConstantsStatic & {
    Config: {
        create: (buffer: BufferSource) => YogaConfig;
    };
};

export default mod;
