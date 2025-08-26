# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## ビルドコマンド
```bash
# ビルドディレクトリの作成とビルド
mkdir -p build && cd build
cmake ..
make

# インストール
sudo make install

# 実行
./clip-template

# クリーンビルド
rm -rf build/*
```

## プロジェクト構造
- Qt5ベースのC++クリップボードテンプレート管理アプリケーション
- CMakeビルドシステム（yaml-cpp、X11依存）
- 設定ファイル優先順位：ユーザー設定（~/.config）→ システム設定（/usr/share）→ ローカル設定（./config）

## アーキテクチャ概要
- **MainWindow**: メインUIコントローラー。キーボードショートカットとウィンドウ管理を担当
- **TemplateManager**: 複数の場所からYAML設定の読み込み/保存を処理
- **ClipboardHandler**: X11サポート付きQtクリップボード統合
- **KeyboardHandler**: 自動ペースト用のX11キーボードイベントシミュレーション
- **ConfigReader**: yaml-cppライブラリを使用したYAML解析
- テンプレート構造: name、content、category、shortcut（0-9）

## 開発時の注意点
- Qt5の標準的なパターンに従う（MOC、シグナル/スロット）
- X11環境での動作が前提
- yaml-cppを使用したYAML処理
- 設定ファイルは複数の場所から読み込み可能