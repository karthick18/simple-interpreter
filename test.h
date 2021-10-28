v = 5;
res = fact (v);
printf ("fact of %d = %d\n", v, res);
fun fact (a)
{
  if (!a || a == 1)
    {
      return 1;
    }
  return a * fact (a - 1);
}
v = "windows" * 2 + fact(2);
printf("v=%s\n", v);
