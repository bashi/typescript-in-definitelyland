= なぜなに！TypeScript！

== TypeScriptって何？

TBD

== まずはインストールしてみよう

Node.jsのセットアップは既に完了しているものとします。
筆者は@<href>{https://github.com/hokaccha/nodebrew#nodebrew,nodebrew}を利用しています。

TypeScriptのインストールには、Node.jsのパッケージマネージャである@<kw>{npm, Node Package Manager}を利用します。
TypeScriptをインストールすると、tscというコマンドが利用可能になり、これでTypeScriptコードのコンパイルを行います。

//cmd{
# -g をつけるとグローバルなコマンドとしてインストールする
$ npm install -g typescript
省略
$ tsc -v
message TS6029: Version 1.1.0.1
$ echo "class Sample {}" > sample.ts
$ tsc --noImplicitAny sample.ts
$ cat sample.js
var Sample = (function () {
    function Sample() {
    }
    return Sample;
})();
//}

これで準備は整いました。

cutting edgeな最新版コンパイラを利用したい場合は以下の手順で行います。

//cmd{
$ npm install -g Microsoft/TypeScript
省略
$ tsc -v
message TS6029: Version 1.3.0.0
//}

未リリースのTypeScriptを常用するのは怖いので、一般的にはプロジェクトローカルで利用するのがよいでしょう。
その方法を以下に示します。

//cmd{
$ npm init
Enter連打
$ ls package.json                                                                                                                package.json
$ npm install Microsoft/TypeScript --save-dev
$ echo "class Sample {}" > sample.ts
$ ./node_modules/.bin/tsc --noImplicitAny sample.ts
$ cat sample.js
var Sample = (function () {
    function Sample() {
    }
    return Sample;
})();
//}

このやり方の場合、npm installする時に200MBを優に超えるリポジトリをまるまるcloneしてこようとするため、ものすごく処理に時間がかかります。
複数人で作業するようなプロジェクトには素直にリリースされているバージョンを利用するのがよいでしょう。

== WebStormで開発環境を整えよう

TBD

https://youtrack.jetbrains.com/issue/WEB-14149
https://youtrack.jetbrains.com/issue/WEB-14151

== 基本構文を確認しよう

TBD
