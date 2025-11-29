export type RequestAnswers = {
  answers: {
    kanjiID: number,
    isCorrect: boolean,
  }[]
}

export type KanjiWord = {
  word: string,
  reading: string 
};

export type KanjiData = {
  id: number,
  kanji: string,
  examples: KanjiWord[],
  meaning: string,
};

export type ResponseKanjis = KanjiData[];

export class Transport {
    foo: number = 0;
    public sendAnswers(req: RequestAnswers): Promise<void> {
        console.log('Sending answers:', req);
        ++this.foo;
        return Promise.resolve();
    }

    public async getKanjis(): Promise<ResponseKanjis> {
        const result = await (window as any).GetKanjis();
        alert(JSON.stringify(result));
        return result;
    }
}