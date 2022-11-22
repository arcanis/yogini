import {ConstantsStatic, YogaConfig} from './yoga';
export type * from './yoga';

export function createContext(wasm: BufferSource): Promise<YogaConfig>;

declare const Yoga: ConstantsStatic;
export default Yoga;
