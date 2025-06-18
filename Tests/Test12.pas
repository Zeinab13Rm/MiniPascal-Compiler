// Test 12: Simple Procedure Call
// Tests defining and calling a procedure with no parameters.

PROGRAM TestTwelve;
PROCEDURE DoPrint;
BEGIN
  writeln('Inside procedure');
END;

BEGIN
  DoPrint;
END.

// Expected Output: Inside procedure