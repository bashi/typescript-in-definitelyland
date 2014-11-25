= TypeScriptの型システム

#@# TODO 構造的部分型
#@# TODO 文脈依存型
#@# TODO 文字列により特殊化されたオーバーロード
#@# TODO Generics
#@# TODO 型クエリ
#@# TODO 再帰型 https://github.com/Microsoft/TypeScript/blob/master/doc/spec.md#3.10.7
#@# TODO プロパティの文字列定数によるアクセス
#@# TODO || 演算子あたりのだるさ
#@# TODO 関数 アロー関数式

っしゃオラー！
TypeScriptの華はやはり型！
@<chapref>{typescript-basic}など所詮児戯に過ぎぬわッ！！

というわけで、TypeScriptの型周りについて解説していくよ。

== 型宣言空間と変数宣言空間

TBD
訳語がアレな気がするので一回specを確認する

== インタフェース

TBD

== オブジェクト型リテラル (object type literal)

TBD

== 共通最適型 (Best Common Type)

#@# [1.3.0での仕様](https://github.com/Microsoft/TypeScript/blob/release-1.3/doc/spec.md#3.10)

@<strong>{導入されたバージョン 1.0.0より前}

@<strong>{消滅したバージョン 1.4.0}

後述の tuple types, 及び union types が導入されたので、Best Common Typeの概念がどうなったのか調べてみてびっくりしました。
現在の最新仕様ではBCTという概念そのものが消滅してますね…。
こぇー…。
本書執筆時点では1.3.0しかリリースされていないため、一応現行では生きてる仕様なのですが。
まぁ、そりゃunion typesに置き換えられてしまいますよね…。

#@# TODO 最新の記述にあわせる
#@# TODO 本校執筆時 https://github.com/Microsoft/TypeScript/blob/9a5df8585bdb46427074b53fc0e46bc4f52dd261/doc/spec.md#4.12.2

まぁ、消える仕様のことを書いても詮無いことなので、ここではざっくり書くにとどめます。

Best Common Type の名の響き通り、複数要素の間で型の統一がされない場合、共通最適型のアルゴリズムの元に型が決定されていました。
例えば、@<list>{bct-basic-1.3.0}のような感じ。

//list[bct-basic-1.3.0][基本的な例]{
#@mapfile(../code/with-types/bct-basic-1.3.0.ts)
// {}[] でございます。
var array = [1, true];

// No best common type exists among return expressions. と怒られます。
// つまり 返り値が {} に推論されたので怒られた。
function test() {
    if (Math.random() < 0.5) {
        return 1;
    } else {
        return true;
    }
}
#@end
//}

1つ目は配列の要素の型が一致しないため、BCTを求めた結果、共通のsuper typeがなかったので {} になっています。
2つ目は関数のreturnステートメントが2つありますが、両者で型が一致しなかったので {} になり、結果コンパイルエラーとして怒られています。

本当に共通の要素がある場合、それに収束したりします。
例えば、親クラスA、その子クラスB, Cがある場合、BCTはAになります。

//list[bct-class-1.3.0][あまり見かけないBCTが役に立つ例]{
#@mapfile(../code/with-types/bct-class-1.3.0.ts)
class A { }
class B extends A {
    str: string;
}
class C extends A {
    num: number;
}

// array: A[] だ！
var array = [new A(), new B(), new C()];
#@end
//}

#@# TODO なお、master/HEADでもarrayは A[] になる模様。 (A | B | C)[] ではない。仕様をよく読むこと。

以上、解散！

== タプル型 (tuple types)

#@# http://qiita.com/vvakame/items/0b5060de5566f210479b

#@# [作業場所](https://github.com/Microsoft/TypeScript/pull/428)
#@# [仕様1](https://github.com/Microsoft/TypeScript/blob/release-1.3/doc/spec.md#3.3.3)
#@# [仕様2](https://github.com/Microsoft/TypeScript/blob/release-1.3/doc/spec.md#3.6.5)

@<strong>{導入されたバージョン 1.3.0}

tuple(たぷる)は、任意の数の要素の組です。
JavaScriptではtupleはサポートされていないため、TypeScriptでのtupleはただのArrayです。

既存のJavaScriptの資産を使おうとした時に、配列の形で多値を返してくるライブラリが稀にあります。
タプル型はおそらくそういった時に使うためのもので、TypeScriptでコードを書く時に多様するものではないでしょう。
というのも、普通にコードを書いている限りでは型推論の結果としてタプル型が出てこないためです。

タプル型は型の世界でしか登場しません。
記述方法は配列の型指定に @<code>{[typeA, typeB]} のように配列の要素の代わりに型名を記述していくだけです。
記述例を見てみましょう@<list>{tuple1}。

//list[tuple1][基本的な例]{
#@mapfile(../code/with-types/tuple1.ts)
// まずは今までどおりの配列から
// TypeScript 1.3.0 ではこれはいままで通りの {}[]
// TypeScript 1.4.0? ではこれは別の箇所で解説しているunion typesとあわせて(string | number | boolean)[]
var array = [1, "str", true];

// {} は charAt を持たないので下記はコンパイルエラーになる
// array[1].charAt(0);

// tuple! 明示的な型の指定が必要
var tuple: [number, string, boolean] = [1, "str", true];

// string は charAt を持つ！
tuple[1].charAt(0);

// TypeScriptのtuple typesは普通にArrayでもあるのだ
tuple.forEach(v => {
    console.log(v);
});
#@end
//}

各要素の型を指定してやると、その要素のindexでアクセスした時に適切な型で扱われるようになります。

もちろん、タプル型はGenericsと組み合わせて利用することができます(@<list>{tuple3})。

//list[tuple3][Genericsでの利用も可]{
#@mapfile(../code/with-types/tuple3.ts)
// Genericsを使ってtupleを生成して返す
function zip<T1, T2>(v1: T1, v2: T2): [T1, T2] {
    return [v1, v2];
}

var tuple = zip("str", { hello: () => "Hello!" });
tuple[0].charAt(0); // おー、静的に検証される！
tuple[1].hello(); // おー、静的に検証される！
#@end
//}

Good!いいですね。

タプル型のない今まで(1.1.0-1以前)のTypeScriptでは@<list>{without-tuple}のような、数値添字で型安全 or 配列として利用可能 という究極の選択をしなければならなかったですからね…。
辛かった。

//list[without-tuple][TypeScript 1.1.0-1 までの苦しい運用]{
#@mapfile(../code/with-types/tuple3.ts)
// Genericsを使ってtupleを生成して返す
function zip<T1, T2>(v1: T1, v2: T2): [T1, T2] {
    return [v1, v2];
}

var tuple = zip("str", { hello: () => "Hello!" });
tuple[0].charAt(0); // おー、静的に検証される！
tuple[1].hello(); // おー、静的に検証される！
#@end
//}

さて、タプル型の重箱の隅を見て行きましょう。
要素数が多すぎる場合、指定されていない値の型はBCTになります。
その例を見てみましょう(@<list>{tuple2})。
TODO 1.4.0でのBCTの扱いが正しいか確認する

//list[tuple2][値の要素数が多すぎる場合]{
#@mapfile(../code/with-types/tuple2.ts)
// 要素が多い分にはOKだ！
var tuple: [string, number] = ["str", 1, "test"];

// 1.4.0? では範囲外の要素の型は全ての要素のunion、つまりここでは string | number になる。
// 1.3.0 では型指定されていない要素は BCT(Best Common Type) つまりここでは {} になる
var value = tuple[2];

// 1.3.0 では以下の記述は正しい
// 1.4.0? ではダメ。true は string | number ではないため。
// tuple = ["str", 1, true];
#@end
//}

お次は要素の順序がズレた場合、どうなるかを見てみましょう(@<list>{tuple-unshift})。

//list[tuple-unshift][絶望に身をよじれ…！]{
#@mapfile(../code/with-types/tuple-unshift.ts)
var tuple: [string, number] = ["str", 1];

// 先頭を number に…
tuple.unshift(1);

// あぁっ！実行時エラー！
// Uncaught TypeError: undefined is not a function
tuple[0].charAt(0);
#@end
//}

…悲しい結果になりました。
@<code>{[1, true]}のような配列のリテラルをtuple typesに推論しないのはおそらくこういった事情でしょう。

unshiftやpopなど、配列の要素を操作する方法は色々ありますが、後からprototypeを拡張することすら可能なJavaScriptではTypeScriptコンパイラ側で全てをフォローすることは不可能です。
タプル型を扱う場合は要素数を変更するような操作をしないほうがよいでしょう。

TypeScript 1.3.0ではもうちょっと辛いコードを書くこともできます(@<list>{tuple-unshift-1.3.0})。

//list[tuple-unshift-1.3.0][1.3.0だともっと辛い]{
#@mapfile(../code/with-types/tuple-unshift-1.3.0.ts)
// 1.3.0 限定！！
var tuple: [string, number] = ["str", 1];

// 先頭を boolean に…
// unshiftの定義は Array<T>.unshift(...items: T[]): number
// 1.3.0 の場合 T が {} なのでコンパイルが通ってしまう
tuple.unshift(true);

// あぁっ！実行時エラー！
// Uncaught TypeError: undefined is not a function
tuple[0].charAt(0);
#@end
//}

結論：タプル型を過信するのはやめろ繰り返すタプル型を過信するのはやめろ！

== 直和型 (union types)

@<strong>{導入されるバージョン 1.4.0}

はい、皆様待望の機能でございます。
言ってはならないあの界隈がよく使う用語を使って解説しないといけないのでビクビクですね。

一番最初に書いておくけど@<strong>{積極的にTypeScriptのコード書く時に使うもんじゃないぞ！！}

じゃあ解説していきましょう。
union typesはいわゆる直和型でございます。
この変数の値は、アレか、コレか、ソレ！のどれか。
どれかは知らん。
みたいな感じ。

なんのために入ったのか？というと、既存JavaScriptにより良い型定義を与えるために入った…！！と言ってしまっていいでしょう。
実際、自分でTypeScriptコード書いてる時は欲しくならないしね。
ECMAScriptさん、パターンマッチもないしー。みたいなー(@<list>{union-types-basic})。

//list[union-types-basic][型A | 型B → 新食感！]{
#@mapfile(../code/with-types/union-types-basic.ts)
var a: string | boolean;
// string | number なので以下はオッケー！
a = "str";
a = true;
// number はアカン。
// error TS2322: Type 'number' is not assignable to type 'string | boolean'.
// Type 'number' is not assignable to type 'boolean'.
// a = 1;

var b: boolean | number;
// a と b を合体させると string | boolean | boolean | number だ！
// 実際にはc の型は string | number | boolean となる
var c: typeof a | typeof b;
#@end
//}

ハイ、型注釈で複数の型を | で区切って書ける感じです。
既存のJavaScriptライブラリだとこういう感じの困った返り値の関数、ちょいちょいありますね。
あとは普通にTypeScriptを書いている時でもSyntaxTreeとかをコードから構築する時にはあったほうが便利かもしれません。

ご覧のとおり、union types中の型の順番とかは関係ない(交換可能)だし、union typesのunion typesとかは単純にまとめられます。
次に見る@<list>{union-types-subtype}のように、union typesに含まれる型同士が親子関係にある場合、単に親にまとめられます。
これも実用上問題ありません。
というのも、@<hd>{type-guards}で紹介する仕組みがあるからです(後で読んでね！)。

//list[union-types-subtype][要素Bが要素Aのサブタイプの場合Aにまとまる]{
#@mapfile(../code/with-types/union-types-subtype.ts)
class Base {
    str: string;
}
class Inherit extends Base {
    num: number;
}

// 明示的に指定しとくと Base | Inherit となる(今のところ
var obj: Base | Inherit;

// Base にも Inherit にも存在する要素には普通にアクセスできる
obj.str;
// num は Base には存在しないのでアウトー！
// error TS2339: Property 'num' does not exist on type 'Base | Inherit'.
// obj.num;

// Base | Inherit は実質単に Base とするのと変わらないのでこんなんやると Base に丸められる
var base: typeof obj;
// まぁ型が Base でも子クラスだから普通に代入できるけどね
base = new Inherit();
#@end
//}

この辺り、仕様書上は若干小難しく書かれているのですが、単に最も少ない要素数になるように型がまとめられていくだけです。

自然にTypeScriptを書いていて、union typesを目にする機会は3種類あります。
|| 演算子を使った時、条件(三項)演算子を使った時、配列リテラルを使った時です(@<list>{union-types-infered})。

//list[union-types-infered][要素Bが要素Aのサブタイプの場合Aにまとまる]{
#@mapfile(../code/with-types/union-types-infered.ts)
// and の型は string | boolean
var and = "str" || true;
// cond の型は string | number
var cond = true ? 1 : "str";
// array の型は (string | number | boolean)[]
var array = [1, true, "str"];
#@end
//}

一番よくお目にかかるのは配列リテラルでしょうか。
TypeScript一般のベストプラクティスとして1つの配列に複数の型の値をツッコまないほうが堅牢なコードになるため、そこをしっかり守るとあんま見ないかもしんない。

型注釈として関数を与える時は記法にちょっと気をつけないとコンパイルエラーになります(@<list>{union-types-syntax})。

//list[union-types-syntax][型名をカッコで囲うんです？]{
#@mapfile(../code/with-types/union-types-syntax.ts)
// 引数無しの返り値stringな関数 な型注釈
var func: () => string;

// 素直に考えるとこう書けてもいいっしょ！でもダメ！
// var a: () => string | () => boolean;

// 型名をカッコでくくる必要がある。これならOK
var b: (() => string) | (() => boolean);
// もしくはオブジェクト型リテラル使う
var c: { (): string; } | { (): boolean; };

// union typesじゃない時でも使えるけど見づらいな！
var d: (() => string);
#@end
//}

ぶっちゃけ見づらいですよね。
仕様書上でも@<href>{https://github.com/Microsoft/TypeScript/issues/1267,カッコの対応ミスってた}りするので、頑張って気をつけましょう。
まぁ、コンパイルすればわかるしー…みたいなー。

union typesな値を使う時は、一応型アサーションも使えますがなるべくなら避けて通りましょう(@<list>{union-types-type-assertion})。
次に説明する@<hd>{type-guards}を使おう！まずはそっちだ！

//list[union-types-type-assertion][一応使えるよ こうすれば]{
#@mapfile(../code/with-types/union-types-type-assertion.ts)
// 注意！ここでやってるやり方よりもtype guardsを使うんだ…！！
// 型アサーションは悪い。常に悪い。なるべく使わないこと。

var obj: string | number | Date;

// string 扱いしてみる
(<string>obj).charAt(0);

// number 扱いしてみる
(<number>obj).toFixed(2);

// Date 扱いしてみる
(<Date>obj).getTime();

// 値の集合に含まれない型にしてみると普通に怒られる
// error TS2352: Neither type 'string | number | Date' nor type 'RegExp' is assignable to the other.
// (<RegExp>obj).test("test");
#@end
//}

色々試してみるが、期待以上に頭がよかったりはしない(@<list>{union-types-cant-infered-invalid})。

//list[union-types-cant-infered-invalid][こういうのは型推論できない]{
#@mapfile(../code/with-types/union-types-cant-infered-invalid.ts)
function test<T>(...args:T[]):T[] {
    return args;
}
// (number | boolean)[] にはならなかった。残念。
// error TS2453: The type argument for type parameter 'T' cannot be inferred from the usage. Consider specifying the type arguments explicitly.
var v = test(1, true);
#@end
//}

TODO https://github.com/Microsoft/TypeScript/blob/master/doc/spec.md#3.4.1

#@# NOTE http://togetter.com/li/749889
#@# NOTE 代数的データ型 algebraic data type 型を組み合わせて作られる型のこと
#@# NOTE 多相性 (a: any[], b: any) => [].concat(a).concat([b]); みたいな。a, b の型に関係なく関数が操作できる
#@# NOTE 直和型 variant TypeScriptの union types っぽい OCamlのは値が取れたりしてもうちょっと強いので本当に同じかいまいちわからん https://twitter.com/omasanori/statuses/537256100915990529
#@# NOTE 直積型 ??? TypeScriptのtype aliasっぽい…？ type 線 = 点1 * 点2 みたいな たかだか一種類のコンストラクタしかもたないもの(点を2つ取るもののみとか)
#@# NOTE 小クワガタ 黒くて挟む角が2つ生えてる虫

=={type-guards} type guards

@<strong>{導入されるバージョン 1.4.0}

TBD

なんかunion types関係ないinstanceofでもイケそうなので分離した

== 型の別名 (type alias)

@<strong>{導入されるバージョン 1.4.0}

TBD
TODO 再帰的な直和型
TODO 基本union typesが絡む別名の定義以外で使う場面ないはずじゃろ？みたいな話書く

== その他取りこぼし

随時追加すること

 * オーバーロードの選択アルゴリズム
