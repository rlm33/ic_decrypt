require 'benchmark'

$decrypter = "./dist/Debug/GNU-Linux-x86/ic_decrypt"
$downcase_size = 26

$to_test = "alpha gamma light zoned"

def ic_decrypt args
	system("#{$decrypter} #{args}")
end

Benchmark.bm do |x| 
	puts 'DOWNCASE:'
	x.report("Simple:\n") { ic_decrypt($to_test) }
	x.report("2 threads:\n") { ic_decrypt("#{$to_test} -p -n 2") }
	x.report("3 threads:\n") { ic_decrypt("#{$to_test} -p -n 3") }
	x.report("4 threads:\n") { ic_decrypt("#{$to_test} -p -n 4") }
	x.report("#{$downcase_size} threads\n:") { ic_decrypt("#{$to_test} -p -n #{$downcase_size}") }
end