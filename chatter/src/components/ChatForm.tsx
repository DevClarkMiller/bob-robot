import { useForm } from 'react-hook-form';
import { zodResolver } from '@hookform/resolvers/zod';
import { z } from 'zod';
import { Form, FormControl, FormField, FormItem, FormLabel, FormMessage } from './ui/form';
import { Button } from './ui/button';
import { Input } from './ui/input';
import { Card, CardContent } from './ui/card';

import { poster } from 'helios-utilities-sdk';
import { BaseApiUrl } from '@/Constants';
import { swapAndFetchToken } from '@/helpers/TokenHelper';

export interface ChatFormProps {
	unitGuid: string;
}

const ChatForm = ({ unitGuid }: ChatFormProps) => {
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

		const oldToken = swapAndFetchToken('chatterToken');
		await poster(url, null, true);
		form.resetField('message');
		localStorage.setItem('token', oldToken);
	};

	return (
		<Card className="w-100 mx-auto">
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
										<Input placeholder="Heyyy, wyd??? Wanna ft??" {...field} />
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
	);
};

export default ChatForm;
