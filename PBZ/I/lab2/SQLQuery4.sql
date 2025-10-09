-- Task 9 
--Получить номера деталей, поставляемых поставщиком в Лондоне
SELECT DP.Д AS "Номер детали"
FROM "Поставщики S" PS
INNER JOIN "Количество деталей, поставляемых одним поставщиком для одного проекта" KDP
    ON PS.П = KDP.П
INNER JOIN "Детали P" DP
    ON KDP.Д = DP.Д
WHERE PS.Город = 'Лондон';

--Task 13

SELECT PJ.ПР AS "Номер проекта"
FROM "Количество деталей, поставляемых одним поставщиком для одного проекта" KDP
JOIN "Поставщики S" PS
	ON KDP.П = PS.П
JOIN "Проекты J" PJ
	ON KDP.ПР = PJ.ПР
WHERE PS.Город <> PJ.Город

--Task 35
SELECT S.П, P.Д
FROM "Поставщики S" AS S
CROSS JOIN "Детали P" AS P
WHERE NOT EXISTS (
    SELECT 1
    FROM "Количество деталей, поставляемых одним поставщиком для одного проекта" AS KP
    WHERE KP.П = S.П AND KP.Д = P.Д
)

