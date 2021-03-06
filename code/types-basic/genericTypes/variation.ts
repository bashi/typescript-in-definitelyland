// それぞれのTは独立していて、関連性はない

// クラスとジェネリクス
class SampleA<T> {
	constructor(public data: T) {
	}
}
// 使い方
// 引数から推論できるので<string>は省略する場合のほうが多い
let objA = new SampleA<string>("str");

// インタフェースとジェネリクス
interface SampleB<T> {
	data: T;
}
// 使い方
let objB: SampleB<number> = { data: 1 };

// オブジェクト型リテラル各種
let obj: {
	new <T>(value: T): any;
	<T>(value: T): any;
	methodA<T>(value: T): any;
	// 型パラメータは複数あってもよい
	methodB<T, U>(value: T): U;
};

// 関数型
let func: <T>(array: T[]) => T;
// 使い方
func = <T>(array: T[]) => array[0];
func<number>([1, 2, 3]);

// コンストラクタ型
let ctor: new <T>(value: T) => any;
// 使い方
ctor = SampleA;
new ctor<string>("str");

// type alias
type SampleC<T> = { data: T; };
let objC: SampleC<number> = { data: 1 };

export { SampleA, objA, SampleB, objB, obj, SampleC, objC }
