import {ConstantsStatic, YogaConfig} from './yoga';
export type * from './yoga';

export function createContext(wasm: BufferSource): YogaConfig;

declare const Yoga: ConstantsStatic;
export default Yoga;
