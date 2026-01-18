import { useForm } from 'react-hook-form';
import { zodResolver } from '@hookform/resolvers/zod';
import { z } from 'zod';
import { Card, CardContent, CardHeader, CardTitle } from './ui/card';
import { Form, FormControl, FormField, FormItem, FormLabel, FormMessage } from './ui/form';
import { Input } from './ui/input';
import { Button } from './ui/button';
import { BaseApiUrl } from '@/Constants';
import { poster } from 'helios-utilities-sdk';
import { swapAndFetchToken } from '@/helpers/TokenHelper';

export interface UnitKeyboardProps {
	unitGuid: string;
}

const UnitKeyboard = ({ unitGuid }: UnitKeyboardProps) => {
	const formSchema = z.object({
		message: z
			.string()
			.min(1, { message: 'Message must be at least one character' })
			.max(128, { message: 'Message has a max of 128 characters' })
			.nonempty(),
	});

	const form = useForm<z.infer<typeof formSchema>>({
		resolver: zodResolver(formSchema),
		defaultValues: {
			message: '',
		},
	});

	const onSubmit = async (values: z.infer<typeof formSchema>) => {
		const route = `${BaseApiUrl}/Chat`;
		const url = new URL(route);

		url.searchParams.append('message', values.message);
		url.searchParams.append('unitGuid', unitGuid);

		const oldToken = swapAndFetchToken('unitToken');
		await poster(url, null, true);
		form.resetField('message');
		localStorage.setItem('token', oldToken);
	};

	return (
		<div>
			<Card className="w-75 mx-auto">
				<CardHeader>
					<CardTitle className="text-2xl">Send Message to Bob</CardTitle>
				</CardHeader>
				<CardContent>
					<Form {...form}>
						<form onSubmit={form.handleSubmit(onSubmit)} className="space-y-8">
							<FormField
								control={form.control}
								name="message"
								render={({ field }) => (
									<FormItem>
										<FormLabel>Message</FormLabel>
										<FormControl>
											<Input placeholder="Hi Bob! What's up?" {...field} />
										</FormControl>
										<FormMessage />
									</FormItem>
								)}
							/>
							<Button type="submit">Submit</Button>
						</form>
					</Form>
				</CardContent>
			</Card>
		</div>
	);
};

export default UnitKeyboard;
