using System;

namespace FastDuino.Parser
{
    public static class StringHelper
    {
        public static string Replace(this string src, string find, string before, string after, Func<string, string> target)
        {
            if (string.IsNullOrEmpty(src))
                return src;
            var index = src.IndexOf(find, StringComparison.Ordinal);
            if (index < 0)
                return src;
            var start = !string.IsNullOrEmpty(before) 
                ? src.LastIndexOf(before, index, index, StringComparison.Ordinal) 
                : index;
            if (start < 0)
                return src;
            var end = index + find.Length;
            if (!string.IsNullOrEmpty(after))
            {
                end = src.IndexOf(after, end, StringComparison.Ordinal);
                if (end < 0)
                    return src;
                end += after.Length;
            }
            var middle = src.Substring(start, end - start);
            return src.Substring(0, start) + target(middle) + src.Substring(end);
        }

        public static string Replace(this string src, string find, string before, string after, string target)
        {
            return src.Replace(find, before, after, x => target);
        }

    }
}