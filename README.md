# Clip Template

KDE向けのテンプレートペーストアプリケーション。キーボードのみで操作可能で、事前に登録したテンプレートを素早く選択してペーストできます。

## 機能

- 📋 YAMLファイルでテンプレート管理
- ⌨️ キーボードのみで完結する操作
- 🔍 インクリメンタルサーチ機能
- 🚀 数字キーでクイック選択
- 🎨 半透明のモダンなUI
- 📁 自動ペースト機能

## 必要なパッケージ

### Ubuntu/Debian
```bash
sudo apt install build-essential cmake qt5-default libqt5x11extras5-dev libyaml-cpp-dev libx11-dev libxtst-dev
```

### Arch Linux
```bash
sudo pacman -S base-devel cmake qt5-base yaml-cpp libx11 libxtst
```

### Fedora
```bash
sudo dnf install gcc-c++ cmake qt5-qtbase-devel yaml-cpp-devel libX11-devel libXtst-devel
```

## ビルド方法

```bash
# リポジトリをクローン
git clone https://github.com/moezakura/clip-template.git
cd clip-template

# ビルドディレクトリを作成
mkdir build
cd build

# CMakeでビルド設定
cmake ..

# ビルド
make

# インストール（オプション）
sudo make install
```

## 使い方

### アプリケーションの起動
```bash
./clip-template
```

### キーボードショートカット

| キー | 動作 |
|------|------|
| `↑` / `↓` | リスト内を移動 |
| `Enter` | 選択したテンプレートをコピー＆ペースト |
| `Esc` | アプリケーションを終了 |
| `/` | 検索ボックスにフォーカス |
| `1`-`9` | 対応する番号のテンプレートを即座に選択・ペースト |
| `Tab` | 検索ボックスとリスト間でフォーカス移動 |

## 設定ファイル

テンプレートは以下の優先順位で読み込まれます：

1. `~/.config/clip-template/templates.yaml` (ユーザー設定)
2. `/usr/share/clip-template/templates.yaml` (システム設定)
3. `./config/templates.yaml` (ローカル設定)

### 設定ファイルの形式

```yaml
templates:
  - name: "テンプレート名"
    content: "テンプレートの内容"
    category: "カテゴリ名"  # オプション
    shortcut: 1              # 1-9の数字キー

  - name: "複数行テンプレート"
    content: |
      1行目
      2行目
      3行目
    category: "multiline"
    shortcut: 2
```

## カスタマイズ

### テンプレートの追加

1. `~/.config/clip-template/templates.yaml` を編集
2. 新しいテンプレートを追加
3. アプリケーションを再起動

### ショートカットキーの設定

各テンプレートに `shortcut: 数字` を設定することで、1-9の数字キーで即座に選択・ペーストできます。

## トラブルシューティング

### ペーストが動作しない場合

- X11環境で実行していることを確認してください
- `libXtst` がインストールされていることを確認してください
- アプリケーションに適切な権限があることを確認してください

### 設定ファイルが読み込まれない場合

- YAMLファイルの構文が正しいことを確認してください
- ファイルのパスと権限を確認してください

## ライセンス

MIT License

## 貢献

プルリクエストを歓迎します。大きな変更の場合は、まずissueを開いて変更内容を議論してください。
