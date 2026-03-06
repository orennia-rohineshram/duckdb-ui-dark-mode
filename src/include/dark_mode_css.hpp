#pragma once

#include <string>

namespace duckdb {

// Dark mode CSS overrides for the DuckDB UI.
// The remote UI (ui.duckdb.org) defines design tokens as CSS custom properties
// on the `.t0ok310` class. This stylesheet overrides those tokens for dark mode.

static const char *DARK_MODE_CSS = R"CSS(
/* ── DuckDB UI Dark Mode ── */
.t0ok310 {
  /* Gray scale – invert light↔dark */
  --wdf1tn4: #0d0d0e;
  --wdf1tn5: #111214;
  --wdf1tn6: #1a1b1e;
  --wdf1tn7: #262830;
  --wdf1tn8: #343840;
  --wdf1tn9: #9ca3af;
  --wdf1tna: #8b929e;
  --wdf1tnb: #9ca3af;
  --wdf1tnc: #b0b7c3;
  --wdf1tnd: #d1d5db;
  --wdf1tne: #e5e7eb;
  --wdf1tnf: #f3f4f6;

  /* Primary indigo – brighten for dark backgrounds */
  --wdf1tng: #0e1018;
  --wdf1tnh: #141828;
  --wdf1tni: #1a2238;
  --wdf1tnj: #1f2a48;
  --wdf1tnk: #2a3860;
  --wdf1tnl: #8f9eea;
  --wdf1tnm: #7b86e4;
  --wdf1tnn: #6b6eda;
  --wdf1tno: #5d5ec4;

  /* Red / error */
  --wdf1tns: #2a1515;
  --wdf1tnt: #3a1c1c;
  --wdf1tnu: #4a2222;
  --wdf1tnv: #6a3030;
  --wdf1tnw: #f06060;
  --wdf1tnx: #f06060;
  --wdf1tny: #e04a42;
  --wdf1tnz: #dc4040;

  /* Blue / info */
  --wdf1tn13: #0a1a2a;
  --wdf1tn14: #0f2640;
  --wdf1tn15: #153560;
  --wdf1tn16: #1a4580;

  /* Teal / success */
  --wdf1tn1c: #0a1a1a;
  --wdf1tn1d: #0f2828;

  /* Amber / warning */
  --wdf1tn1m: #1a1508;
  --wdf1tn1n: #2a200a;
  --wdf1tn1o: #3a2c0e;

  /* Overlays – flip black→white for dark surfaces */
  --wdf1tn1x: rgba(255, 255, 255, 0.05);
  --wdf1tn1y: rgba(255, 255, 255, 0.08);
  --wdf1tn1z: rgba(255, 255, 255, 0.12);
  --wdf1tn20: rgba(255, 255, 255, 0.15);
  --wdf1tn21: rgba(255, 255, 255, 0.18);
  --wdf1tn22: rgba(255, 255, 255, 0.22);
  --wdf1tn23: rgba(255, 255, 255, 0.30);
  --wdf1tn24: rgba(255, 255, 255, 0.35);
  --wdf1tn25: rgba(255, 255, 255, 0.38);
  --wdf1tn26: rgba(255, 255, 255, 0.40);
  --wdf1tn27: rgba(255, 255, 255, 0.45);
  --wdf1tn28: rgba(255, 255, 255, 0.50);
  --wdf1tn29: rgba(255, 255, 255, 0.60);

  /* Semantic / component tokens */
  --wdf1tn2a: #7b86e4;
  --wdf1tn2b: #5d5ec4;
  --wdf1tn2c: #141518;
  --wdf1tn2d: #1a1b1e;
  --wdf1tn2e: #2a2c31;
  --wdf1tn2f: #2a2c31;
  --wdf1tn2g: #343840;
  --wdf1tn2h: hsl(222.2 47.4% 90%);
  --wdf1tn2i: #f06060;
  --wdf1tn2j: #e04a42;
  --wdf1tn2k: #141518;
  --wdf1tn2l: #2a1515;
  --wdf1tn2m: #3a1c1c;
  --wdf1tn2n: #dc4040;
  --wdf1tn2o: hsl(214.3 45.8% 20%);
  --wdf1tn2p: hsl(215.4 16.3% 50%);
  --wdf1tn2q: rgba(255, 255, 255, 0.08);
  --wdf1tn2r: rgba(255, 255, 255, 0.12);
  --wdf1tn2s: rgba(255, 255, 255, 0.18);
  --wdf1tn2t: rgba(255, 255, 255, 0.22);
  --wdf1tn2u: hsl(215 20.2% 50%);
  --wdf1tn2v: hsl(0 0% 10%);
  --wdf1tn2w: hsl(222.2 47.4% 90%);
  --wdf1tn2x: #e5e7eb;
  --wdf1tn2y: #141518;
  --wdf1tn2z: #e5e7eb;
  --wdf1tn30: #9ca3af;
  --wdf1tn31: #8b929e;
  --wdf1tn32: #8b929e;
  --wdf1tn33: #f06060;
  --wdf1tn34: #1f2024;
  --wdf1tn35: #2a2c31;
  --wdf1tn36: color-mix(in srgb, #111214 50%, #1a1b1e 50%);
  --wdf1tn37: #343840;
  --wdf1tn38: #b0b7c3;
  --wdf1tn39: hsl(0 0% 10%);
  --wdf1tn3a: hsl(0 0% 10% / 0.7);
  --wdf1tn3b: hsl(222.2 47.4% 90%);
  --wdf1tn3c: #111214;
  --wdf1tn3d: rgba(0, 0, 0, 0.5);
  --wdf1tn3e: #1c1a15;
  --wdf1tn3f: #161616;
  --wdf1tn3g: #ffffff;
  --wdf1tn3m: rgba(0, 0, 0, 0.7);
  --wdf1tn3n: rgba(0, 0, 0, 0.8);

  /* Shadows – stronger on dark backgrounds */
  --wdf1tn6e: 0 1px 2px 0 rgb(0 0 0 / 0.2);
  --wdf1tn6f: 0 1px 3px 0 rgb(0 0 0 / 0.3), 0 1px 2px -1px rgb(0 0 0 / 0.25);
  --wdf1tn6g: 0 4px 6px -1px rgb(0 0 0 / 0.3), 0 2px 4px -2px rgb(0 0 0 / 0.25);
  --wdf1tn6h: 0 10px 15px -3px rgb(0 0 0 / 0.3), 0 4px 6px -4px rgb(0 0 0 / 0.2);
  --wdf1tn6i: 0 20px 25px -5px rgb(0 0 0 / 0.3), 0 8px 10px -6px rgb(0 0 0 / 0.2);
  --wdf1tn6j: 0 25px 50px -12px rgb(0 0 0 / 0.5);
  --wdf1tn6k: inset 0 2px 4px 0 rgb(0 0 0 / 0.15);
  --wdf1tn6m: rgba(0, 0, 0, 0.5) 0px 16px 170px;
}

html, body {
  background-color: #0f1117 !important;
  color: #e5e7eb !important;
  color-scheme: dark;
}

::-webkit-scrollbar { width: 8px; height: 8px; }
::-webkit-scrollbar-track { background: #1a1b1e; }
::-webkit-scrollbar-thumb { background: #343840; border-radius: 4px; }
::-webkit-scrollbar-thumb:hover { background: #4b5563; }
::selection { background: rgba(100, 112, 221, 0.35); color: #e5e7eb; }

/* ── CodeMirror / SQL editor overrides ── */
/* The UI's CodeMirror theme uses high-specificity scoped selectors
   (e.g. .ͼ13 .cm-scroller .cm-gutters { background-color: white })
   that hardcode light-mode colors. We use !important to override them. */

.cm-editor {
  color: #e5e7eb !important;
}

.cm-scroller .cm-gutters,
.cm-gutters {
  background-color: #1a1b1e !important;
  color: #8b929e !important;
  border-right-color: #2a2c31 !important;
}

.cm-gutters .cm-lineNumbers,
.cm-lineNumbers {
  color: rgba(155, 165, 180, 0.7) !important;
}

.cm-gutters .cm-lineNumbers .cm-gutterElement,
.cm-lineNumbers .cm-gutterElement {
  color: rgba(155, 165, 180, 0.7) !important;
}

.cm-activeLineGutter {
  background-color: rgba(255, 255, 255, 0.06) !important;
  color: #b0b7c3 !important;
}

.cm-activeLine {
  background-color: rgba(255, 255, 255, 0.04) !important;
}

.cm-cursor,
.cm-cursor-primary {
  border-left-color: #e5e7eb !important;
}

.cm-selectionBackground,
.cm-editor .cm-selectionBackground,
.cm-focused .cm-selectionBackground,
::selection {
  background-color: rgba(100, 112, 221, 0.35) !important;
}

/* Autocomplete / tooltip */
.cm-tooltip {
  background-color: #1a1b1e !important;
  color: #e5e7eb !important;
  border-color: #343840 !important;
}

.cm-tooltip-autocomplete ul li[aria-selected] {
  background-color: #2a2c31 !important;
  color: #e5e7eb !important;
}

.cm-tooltip.cm-tooltip-autocomplete {
  background-color: #1a1b1e !important;
  border: 1px solid #343840 !important;
}

/* Search panel */
.cm-panels {
  background-color: #1a1b1e !important;
  color: #e5e7eb !important;
  border-color: #343840 !important;
}

.cm-search input,
.cm-search button {
  background-color: #262830 !important;
  color: #e5e7eb !important;
  border-color: #343840 !important;
}

/* Matching brackets */
.cm-matchingBracket {
  background-color: rgba(100, 112, 221, 0.25) !important;
  color: #e5e7eb !important;
}

/* Fold gutter */
.cm-foldGutter .cm-gutterElement {
  color: #8b929e !important;
}

/* Running statement gutter marker */
.cm-running-block-marker,
.cm-running-statement-gutter {
  color: #8b929e !important;
}
)CSS";

// Build the <style> block to inject into HTML <head>.
// mode "auto" → wrap in @media(prefers-color-scheme:dark)
// mode "on"   → apply unconditionally
// mode "off"  → return empty string
inline std::string BuildDarkModeStyleBlock(const std::string &mode) {
	if (mode == "off") {
		return "";
	}
	std::string css(DARK_MODE_CSS);
	if (mode == "auto") {
		return "<style id=\"duckdb-dark-mode\">@media(prefers-color-scheme:dark){" + css + "}</style>";
	}
	return "<style id=\"duckdb-dark-mode\">" + css + "</style>";
}

} // namespace duckdb
