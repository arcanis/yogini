import Yoga from 'yoga-layout';
export type * from 'yoga-layout';

declare const mod: {
    (buffer: BufferSource): Promise<typeof Yoga>;
    initStreaming(response: Response | PromiseLike<Response>): Promise<typeof Yoga>;
};

export default mod;
