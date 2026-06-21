#!/usr/bin/env python3
"""Generate consolidated ai_playerbot_texts SQL from C++ keys and existing updates."""

from __future__ import annotations

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SRC = ROOT / "src"
UPDATES = ROOT / "data/sql/playerbots/updates"
OUTPUT = UPDATES / "2026_06_21_00_ai_playerbot_texts_consolidated.sql"

TEXT_FILES_TO_REMOVE = [
    # Removed from repo; data merged into 2026_06_21_00_ai_playerbot_texts_consolidated.sql
]

# Source files were merged once. To regenerate, restore deleted SQL from git history
# or parse the consolidated file plus C++ keys.

START_ID = 1740


def split_sql_values(raw: str) -> list:
    parts: list = []
    i = 0
    s = raw.strip()
    while i < len(s):
        while i < len(s) and s[i] in " \t\n\r":
            i += 1
        if i >= len(s):
            break
        if s[i] == "'":
            j = i + 1
            chars: list[str] = []
            while j < len(s):
                if s[j] == "'":
                    if j + 1 < len(s) and s[j + 1] == "'":
                        chars.append("'")
                        j += 2
                    else:
                        break
                else:
                    chars.append(s[j])
                    j += 1
            parts.append("".join(chars))
            i = j + 1
            if i < len(s) and s[i] == ",":
                i += 1
        elif s[i].isdigit() or (s[i] == "-" and i + 1 < len(s) and s[i + 1].isdigit()):
            m = re.match(r"-?\d+", s[i:])
            parts.append(int(m.group(0)))
            i += m.end()
            if i < len(s) and s[i] == ",":
                i += 1
        else:
            break
    return parts


def parse_insert_row(line: str) -> dict | None:
    line = line.strip().rstrip(",")
    if not line.startswith("("):
        return None
    inner = line[1:]
    if inner.endswith(")"):
        inner = inner[:-1]
    parts = split_sql_values(inner)
    if len(parts) < 3:
        return None

    idx = 0
    row_id = None
    if isinstance(parts[0], int) and isinstance(parts[1], str):
        row_id = parts[0]
        idx = 1
    elif not isinstance(parts[0], str):
        return None

    need = idx + 11
    if len(parts) < need:
        return None

    loc = parts[idx + 4 : idx + 12]
    while len(loc) < 8:
        loc.append("")
    return {
        "id": row_id,
        "name": parts[idx],
        "text": parts[idx + 1],
        "say_type": int(parts[idx + 2]),
        "reply_type": int(parts[idx + 3]),
        "loc": loc[:8],
    }


def sql_escape(value: str) -> str:
    return value.replace("'", "''")


def extract_code_keys() -> dict[str, str]:
    pattern = re.compile(
        r'GetLocalizedBotTextOrDefault\s*\(\s*"([^"]+)"\s*,\s*"((?:[^"\\]|\\.)*)"'
    )
    localized_pattern = re.compile(
        r'GetLocalizedBotText\s*\(\s*"([^"]+)"'
    )
    keys: dict[str, str] = {}
    for path in SRC.rglob("*"):
        if path.suffix not in (".cpp", ".h"):
            continue
        content = path.read_text(encoding="utf-8", errors="replace")
        for match in pattern.finditer(content):
            key = match.group(1)
            default = match.group(2)
            default = bytes(default, "utf-8").decode("unicode_escape")
            default = default.replace("\\'", "'")
            keys.setdefault(key, default)
        for match in localized_pattern.finditer(content):
            keys.setdefault(match.group(1), "")

    # Dynamic keys (variable name, defaults from C++).
    keys.setdefault("msg_will_loot", "|cFF000000Will loot %item")
    keys.setdefault("msg_wont_loot", "|c00FF0000Won't loot %item")
    keys.setdefault("error_no_event_owner", "No event owner detected")
    keys.setdefault("msg_ai_reset", "AI was reset to defaults")
    return keys


def load_sql_entries() -> tuple[dict[str, dict], dict[str, int]]:
    entries: dict[str, dict] = {}
    chance: dict[str, int] = {}

    sql_files = sorted(UPDATES.glob("*.sql"))
    for sql_file in sql_files:
        content = sql_file.read_text(encoding="utf-8", errors="replace")
        if "ai_playerbot_texts" not in content:
            continue

        for line in content.splitlines():
            stripped = line.strip()
            if stripped.startswith("(") and "'" in stripped:
                row = parse_insert_row(stripped)
                if row and row["name"]:
                    entries[row["name"]] = row

            update = re.match(
                r"UPDATE\s+`?ai_playerbot_texts`?\s+SET\s+`?text_loc4`?\s*=\s*'((?:''|[^'])*)'\s+"
                r"WHERE\s+`?name`?\s*=\s*'([^']+)'",
                stripped,
                re.IGNORECASE,
            )
            if update:
                zh = update.group(1).replace("''", "'")
                name = update.group(2)
                if name not in entries:
                    entries[name] = {
                        "id": None,
                        "name": name,
                        "text": "",
                        "say_type": 0,
                        "reply_type": 0,
                        "loc": [""] * 8,
                    }
                entries[name]["loc"][3] = zh

        for match in re.finditer(
            r"INSERT INTO ai_playerbot_texts_chance[^;]+VALUES\s*\('([^']+)',\s*(\d+)\)",
            content,
            re.IGNORECASE,
        ):
            chance[match.group(1)] = int(match.group(2))

        for match in re.finditer(
            r"\('([^']+)',\s*(\d+)\)\s*,?\s*$",
            content,
        ):
            if "texts_chance" in content[max(0, match.start() - 200) : match.start()]:
                chance[match.group(1)] = int(match.group(2))

    return entries, chance


def merge_entry(name: str, default_text: str, source: dict | None) -> dict:
    if source is None:
        return {
            "name": name,
            "text": default_text,
            "say_type": 0,
            "reply_type": 0,
            "loc": [""] * 8,
        }

    text = source["text"] or default_text
    loc = list(source["loc"])
    return {
        "name": name,
        "text": text,
        "say_type": source["say_type"],
        "reply_type": source["reply_type"],
        "loc": loc,
    }


def format_row(row_id: int, row: dict) -> str:
    loc_sql = ", ".join(f"'{sql_escape(x)}'" for x in row["loc"])
    return (
        f"({row_id}, '{sql_escape(row['name'])}', '{sql_escape(row['text'])}', "
        f"{row['say_type']}, {row['reply_type']}, {loc_sql})"
    )


def collect_obsolete_names(entries: dict[str, dict], used: set[str]) -> list[str]:
    obsolete_prefixes = ("msg_", "error_")
    obsolete = []
    for name in entries:
        if name in used:
            continue
        if name.startswith(obsolete_prefixes):
            obsolete.append(name)
    return sorted(obsolete)


def generate() -> None:
    code_keys = extract_code_keys()
    sql_entries, sql_chance = load_sql_entries()
    used_names = sorted(code_keys.keys())

    merged = [
        merge_entry(name, code_keys[name], sql_entries.get(name))
        for name in used_names
    ]

    obsolete = collect_obsolete_names(sql_entries, set(used_names))

    lines: list[str] = [
        "-- Consolidated ai_playerbot_texts for GetLocalizedBotTextOrDefault keys.",
        "-- Replaces fragmented text updates from 2025_09_17 through 2026_06_20.",
        "-- Idempotent: DELETE by name before INSERT.",
        "",
    ]

    if obsolete:
        lines.append("DELETE FROM `ai_playerbot_texts` WHERE `name` IN (")
        lines.extend(f"    '{sql_escape(n)}'," for n in obsolete[:-1])
        lines.append(f"    '{sql_escape(obsolete[-1])}'")
        lines.append(");")
        lines.append("")

        lines.append("DELETE FROM `ai_playerbot_texts_chance` WHERE `name` IN (")
        lines.extend(f"    '{sql_escape(n)}'," for n in obsolete[:-1])
        lines.append(f"    '{sql_escape(obsolete[-1])}'")
        lines.append(");")
        lines.append("")

    lines.append("DELETE FROM `ai_playerbot_texts` WHERE `name` IN (")
    for i, name in enumerate(used_names):
        suffix = "," if i < len(used_names) - 1 else ""
        lines.append(f"    '{sql_escape(name)}'{suffix}")
    lines.append(");")
    lines.append("")

    chance_names = sorted(n for n in sql_chance if n in used_names)
    if chance_names:
        lines.append("DELETE FROM `ai_playerbot_texts_chance` WHERE `name` IN (")
        for i, name in enumerate(chance_names):
            suffix = "," if i < len(chance_names) - 1 else ""
            lines.append(f"    '{sql_escape(name)}'{suffix}")
        lines.append(");")
        lines.append("")

    lines.append(
        "INSERT INTO `ai_playerbot_texts` "
        "(`id`, `name`, `text`, `say_type`, `reply_type`, "
        "`text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, "
        "`text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES"
    )

    row_id = START_ID
    for i, row in enumerate(merged):
        suffix = "," if i < len(merged) - 1 else ";"
        lines.append(f"{format_row(row_id, row)}{suffix}")
        row_id += 1

    if chance_names:
        lines.append("")
        lines.append("INSERT INTO `ai_playerbot_texts_chance` (`name`, `probability`) VALUES")
        for i, name in enumerate(chance_names):
            suffix = "," if i < len(chance_names) - 1 else ";"
            lines.append(f"('{sql_escape(name)}', {sql_chance[name]}){suffix}")

    OUTPUT.write_text("\n".join(lines) + "\n", encoding="utf-8")

    missing = [n for n in used_names if n not in sql_entries]
    print(f"Code keys: {len(used_names)}")
    print(f"SQL sources: {len(sql_entries)}")
    print(f"Keys without prior SQL (use C++ default): {len(missing)}")
    print(f"Obsolete alias keys removed: {len(obsolete)}")
    print(f"Chance entries: {len(chance_names)}")
    print(f"Wrote {OUTPUT} ({row_id - START_ID} rows, ids {START_ID}-{row_id - 1})")


if __name__ == "__main__":
    generate()
