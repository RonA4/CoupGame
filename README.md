<div dir="rtl" lang="he">

# מטלה מספר 3 - קו (Coup)

---

במטלה זו מימשתי את המשחק **Coup** – משחק קופסה עבור 2–6 שחקנים, שבו כל שחקן מנסה להדיח את האחרים ולשרוד אחרון במשחק.

במהלך הפיתוח עסקנו בנושאים שנלמדו בהרצאות 5–6 כגון:
- כלל השלושה
- ירושה רגילה וירושה וירטואלית

---

## חוקי המשחק

בתחילת המשחק כל שחקן מקבל תפקיד (קלף). התפקידים מעניקים יכולות ייחודיות. במרכז השולחן יש קופת מטבעות.

בכל תור שחקן יכול לבצע פעולה אחת – לפי תפקידו או פעולה גנרית.

### פעולות כלליות:
- **gather** – קבלת מטבע אחד (ניתן לחסימה ע"י sanction)
- **tax** – קבלת שני מטבעות (חסימה ע"י תפקידים מסוימים)
- **bribe** – תשלום 4 מטבעות לקבלת 3 תורות ברצף
- **arrest** – לקיחת מטבע אחד משחקן אחר (לא ניתן על אותו שחקן פעמיים ברציפות)
- **sanction** – חסימת פעולות כלכליות של שחקן אחר בתשלום 3 מטבעות (gather, tax)
- **coup** – הדחת שחקן מהמשחק בתשלום 7 מטבעות
- **watch** – יכולת של Spy לראות מטבעות ולחסום arrest (לא תור ולא עולה מטבע)
- **invest** – יכולת של Baron להשקיע 3 מטבעות ולקבל 6 (כן נחשב תור)

אם פעולה אינה חוקית – תיזרק חריגה מתאימה.

---

## תפקידים

- **Governor (נציב)** – מקבל 3 מטבעות ב־tax, יכול לחסום tax של אחרים.
- **Spy (מרגל)** – רואה את כמות המטבעות של שחקן אחר, חוסם arrest. לא עולה תור.
- **Baron (ברון)** – יכול להשקיע 3 ולקבל 6. אם הותקף ב־sanction, מקבל מטבע כפיצוי.
- **General (גנרל)** – חוסם coup בתשלום 5 מטבעות. אם נעצר – מקבל בחזרה את המטבע.
- **Judge (שופט)** – חוסם bribe (השחקן מפסיד את כל ה־4 מטבעות). אם הותקף ב־sanction – התוקף משלם עוד מטבע.
- **Merchant (סוחר)** – אם מתחיל תור עם 3 מטבעות או יותר – מקבל מטבע נוסף. אם נעצר – משלם 2 לקופה במקום להעביר 1 לשחקן.

---

## הארות:

- ייתכנו שחקנים עם אותו תפקיד – אין הגבלה.
- שחקן עם 10 מטבעות בתחילת תור **חייב לבצע coup**.
- סדר התורות הוא לפי סדר ההצטרפות למשחק.

---

## לוח המשחק

המשחק מאפשר:
- לדעת בכל רגע מי בתור (`turn`)
- לראות את רשימת השחקנים הפעילים (`players`)
- לזהות את המנצח (`winner`)
- לוודא חוקיות פעולות והתקדמות

---

## ממשק גרפי (GUI)

נבנה בעזרת **SFML**, מאפשר חוויית משחק מלאה:

- **הצגת שחקנים** – כל שחקן מוצג בתיבה צבעונית עם שמו ותפקידו
- **כפתורי פעולה** – gather, tax, bribe, arrest, sanction, coup, watch, invest
- **מצב תור** – בראש המסך מופיע שם השחקן שבתור
- **חסימות בזמן אמת** – כמו בלוח משחק אמיתי
- **הודעת ניצחון** – מוצגת בסיום המשחק עם חלון ייחודי

---

<div dir="rtl" lang="he">

### מבנה הפרויקט

<pre dir="ltr">
EX3_COUP/
├── game/          ← לוגיקת משחק (Game, Player)
├── roles/         ← מחלקות תפקידים (Baron, Spy, Governor וכו')
├── GUI/           ← ממשק גרפי (gui.cpp)
├── test/          ← בדיקות יחידה (doctest)
├── main.cpp       ← קובץ ראשי להרצה
├── makefile       ← קימפול והרצה
└── README.md      ← תיעוד הפרויקט
</pre>

</div>

---

## קובץ Makefile:

- `make Main` – קימפול והרצה
- `make test` – הרצת בדיקות יחידה
- `make valgrind` – בדיקת זליגות זיכרון
- `make clean` – ניקוי קבצים זמניים

</div>
